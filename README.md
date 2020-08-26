# Music tracker for Gamebuino

This repository contains a simple tracker that is able to generate multi-track music. It
is heavily inspired by the tracker in [PICO-8][]. It supports the same instruments and
effects, but is it a bit more primitive, as it is intended for the [Gamebuino][] console.
This device only supports 10-bit audio and its CPU power is limited. Therefore, all
calculations are integer-based and only 8-bit multiplications are used.

The actual integration of the tracker into the Gamebuino library is currently available in
the [music-gen branch of my fork][]. It may end up part of the official library someday,
but that is not the case yet.

# Instructions

You can use this tracker to play songs and sound effects on your Gamebuino. To do so, you need to
know the following:

1. How to specify songs and tunes
2. How to play music on the Gamebuino
3. How to import songs from PICO-8 (optional)

## Notes, tunes, patterns and songs

Songs are specified using a hierarchy classes:

* A `SongSpec` defines a sequence of patterns and can be played by a `SongGenerator` which plays
  the patterns one after the other, and optionally loops.
* A `PatternSpec` consists of one or more tunes and can be played by a `PatternGenerator` which
  plays the tunes in parallel.
* A `TuneSpec` consists of a sequence of notes, and can be played by a `TuneGenerator` which plays
  the notes one after the other, and optionally applies effects.
* A `NoteSpec` specifies a single note.

The next sections provides more details about each.

### Notes

A note is defined by a `NoteSpec` struct and consists of a frequency, volume, wave form and
optional effect. The frequency is specified via the `note` field of type `Note`. This is an enum
specifies a note on the chromatic scale and an octave.

The volume is specified via the integer `vol` field. This should be a value in the range from 0
to 8. A zero volume represents silence. Eight is the maximum volume of a note. The volume scale is
linear.

The wave form of the note is specified via the `wav` enum field of type `WaveForm`. The following
wave forms are supported:
* `TRIANGLE`
* `TILTED_SAW`
* `SAW`
* `SQUARE`
* `PULSE`
* `ORGAN`
* `NOISE`
* `PHASER`

Each wave form sounds different. Instead of me trying to poorly describe each sound, you should
simply experiment to hear how they vary and how each can be used. A few hints won't harm though.
The noise can be used for crash sound effects, but also for percussion. The phaser, as the name
suggests, can be used for shooting sound effects.

Note: The first six wave forms are realized via a simple look-up table. The `NOISE` and `PHASER`
are more computationally intensive though, as both execute additional code to dynamically vary the
frequency. So if the CPU load in your game is high, be careful in your usage of these instruments.

Finally, you can optionally apply an effect. This is specified via the `fx` enum field of type
`Effect`. The following effects are supported:
* `NONE` applies no effect, the default.
* `FADE_IN` fades in the note. The volume starts at zero and is linearly increased to reach the
  specified volume when the note finishes.
* `FADE_OUT` fades out the note. The volume starts as specified but is linearly reduced to zero
  by the time the note finishes.
* `DROP` drops the frequency of the note by one octave.
* `SLIDE` slides the frequency and volume of the note. The note starts with the frequency and
  volume of the previous node and only reaches the note's own frequency and volume when the note
  finishes.
* `VIBRATO` vibrates the frequency. The frequency goes up and down.
* `ARPEGGIO` plays an arpeggio. Instead of a single note, it plays four notes. The four notes
  include the note, as well as three neighbouring notes. The first note of the arpeggio is the
  first of its four-quarter measure. Or put differently, when the note is the n-th one in its
  tune, the first note of the arpeggio is m-th one of the tune, where `m = floor(n / 4)`

  Example: Consider the following tune of eight notes `A B C D E F G A`. When the arpeggio is applied to the third and fifth note, this results in the following tune:
  `AAAA BBBB ABCD DDDD EFGA FFFF GGGG AAAA`. The spaces are only for readability. There is no
  delay between any of the notes.

  Note: All notes in an arpeggio have the same volume and wave form, which is specified by the
  note with the arpeggio effect. For the other notes in the arpeggio, only their frequency is
  used.
* `ARPEGGIO_FAST` as `ARPEGGIO` but faster. It plays the same four notes, but plays each note
  twice (and twice as quickly).

Example:
```cpp
#include "Music.h"

const NoteSpec note = NoteSpec {
    .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE
};
```

### Tunes

A tune is a sequence of notes. All notes in a tune have the same duration. However, you can
create longer notes by repeating the same note. When subsequent notes have the same frequency,
volume and wave form they blend together into a single note, without any pause or other audible
transition.

A tune is specified via a `TuneSpec` struct. The duration of notes is specified in 'ticks' by the
integer `noteDuration` field. A tick is defined to contain ninety samples when sampling at 11025
Hz, and therefore lasts 8.16 ms.

The number of notes in a tune is specified by the `numNotes` field. The actual notes should be
specified by the `notes` field, which should point to an array of notes. The number of notes
should in practice always match the length of the array, and at the very least, never exceed the
length of the array.

A tune can optionally loop. When looping is enabled, after the end of the tune is reached, it
continues at the note where the loop starts, indicated by the `loopStart` field. This can have
any integer value between zero (the entire tune loops) up until `numNotes` (the tune does not
loop). How often a tune loops is implicitly determined by the other tunes in the pattern, as
detailed in the next section.

Optionally, the volume of a tune can be boosted. This can be done by setting the `boostVolume`
field to `true`. Volume is then doubled. Care should be taken not to overflow the ten-bit audio
range this way.

Example:
```cpp
#include "Music.h"

const NoteSpec notes[4] = {
    NoteSpec { .note=Note::A, .vol=3, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
    NoteSpec { .note=Note::B, .vol=4, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
    NoteSpec { .note=Note::C, .vol=5, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
    NoteSpec { .note=Note::D, .vol=5, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
};
const TuneSpec tune = TuneSpec {
    .noteDuration = 12, .loopStart = 4, .numNotes = 4, .notes = notes
};
```

Converting a tune into samples is done by the `TuneGenerator` class. However, this is a low-level
class that you typically do not invoke directly. Instead the easiest way to listen to a tune is to
convert it into a WAV file. This can be done by the `makeWav` helper function in `WavGeneration.h`
that takes a `TuneSpec` reference as second argument. If the tune loops, you should also specify
a maximum number of samples, otherwise it will never terminate.

Example:
```cpp
#include "WavGeneration.h"

makeWav("tune.wav", tune);
```

### Patterns

Patterns consists of multiple tunes, which are played together. One tune could be a bass track,
another the rythm track, and a third the melody. A pattern can contain at most four tunes. The
samples of all tunes are added together. As long as none of the tunes has volume boost enabled
then a pattern will never overflow the 10-bit output range.

A pattern is specified by a `PatternSpec` struct. It contains only two members. The number of
tunes is specified in the `numTunes` field, and the tunes are specified in the `tunes` field.

Example:
```cpp
#include "Music.h"

const TuneSpec tune1 = TuneSpec { /* omitted */ };
const TuneSpec tune2 = TuneSpec { /* omitted */ };
const TuneSpec tune3 = TuneSpec { /* omitted */ };
const TuneSpec *const tunesInPattern[3] = { &tune1, &tune2, &tune3 };
const PatternSpec pattern = PatternSpec { .numTunes = 3, .tunes = tunesInPattern };
```

Note: The first tune in a pattern determines its length. This tune should therefore never loop.
Other than that, the order of the tunes does not matter. So if you have tune that is repetitive,
e.g. a rythm or bass track, you can let it loop so that you do not have to unnecessarily repeat
the notes. This track should then not be the first in the array.

Converting a pattern into samples is done by the `PatternGenerator` class, which you also typically
do not need to invoke directly. Instead, to listen to a pattern, convert it to WAV file via the
`makeWav` helper function that takes a `PatternSpec` as second argument.

Example:
```cpp
#include "WavGeneration.h"

makeWav("pattern.wav", pattern);
```

### Songs

Songs consist of series of patterns, which are played in sequence, and can optionally contain a
loop. A loop can be useful when the song is intended as background music during game play and
should play as long as the player is playing the game.

It's becoming a bit repetitive, but a song is specified by a `SongSpec` struct. The number of
patterns is specified by the `numPatterns` field, and the patterns are specified in the `patterns`
field similar to how this is done for tunes in a patterns. If the song can loop, the start of the
loop should be specified by the `loopStart` field. If it is zero, the song loops in its entirety.
If the song contains an intro that should only be played once, you should set the loop start to
the index of the first pattern after the intro. If the song should never loop, `loopStart` should
be set to `numPatterns`.

Example:
```cpp
#include "Music.h"

const PatternSpec pattern1 = TuneSpec { /* omitted */ };
const PatternSpec pattern2 = TuneSpec { /* omitted */ };
const PatternSpec pattern3 = TuneSpec { /* omitted */ };
const PatternSpec *const patternsInSong[4] = {
    &pattern1, &pattern2, &pattern2, &pattern3
};
const SongSpec song = SongSpec {
    .loopStart = 1, .numPatterns = 4, .patterns = patternsInSong
};
```

As this example shows, it is perfectly fine to repeat patterns in a song.

A song is converted into samples by the `SongGenerator` class, which you typically do not invoke
directly either. Instead, to listen to a song, convert it to a WAV File via the `makeWav` helper
function that takes a `SongSpec` as a second argument.

Example:
```cpp
#include "WavGeneration.h"

makeWav("song.wav", song);
```

## Playings songs and tunes on Gamebuino

Topics:
* Using the patched Gamebuino library
* API (namespace, how to play songs, how to play tunes)
* Recommended settings (sample rate, etc)

TO DO

## Importing songs from PICO-8

Topics:
* Automatic import
* Limitations and differences compared to PICO-8
    * Custom instruments
    * Mismatch in volumes
    * VIBRATO not supported for NOISE
* Post-processing
    * Remove duplicate patterns
    * Use looping for repetitive tunes
    * Remove unused tunes and patterns
    * Ensure pattern does not loop
    * Fix tunes with custom instruments
    * Tweaks to remove noise artifacts
    * Avoid note cancellation

TO DO

[Gamebuino]: https://gamebuino.com
[PICO-8]: https://www.lexaloffle.com/pico-8.php
[music-gen branch of my fork]: https://github.com/erwinbonsma/Gamebuino-META/tree/music-gen