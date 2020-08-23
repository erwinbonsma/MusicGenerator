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

1. How to specify songs and tunes, including capabilities and limitations of the tracker
2. How to test
3. How to import songs from PICO-8 (optional)
4. How to integrate it to the Gamebuino

## Capabilities and limitations

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
optional effect. The frequency specified via the `note` field of type `Note`. This is an enum
specifies a note on the chromatic scale and an octave.

A volume is specified via the integer `vol` field. This should be a value in the range from 0 to 8.
A zero volume represents silence. Eight is the maximum volume of a note. The volume scale is
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


[Gamebuino]: https://gamebuino.com
[PICO-8]: https://www.lexaloffle.com/pico-8.php
[music-gen branch of my fork]: https://github.com/erwinbonsma/Gamebuino-META/tree/music-gen