//
//  ChannelTune.h
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#ifndef ChannelTune_h
#define ChannelTune_h

#include <stdio.h>

constexpr int SAMPLERATE = 22050;
// Should be updated when changing SAMPLERATE as follows:
// 44100 -> 0, 22050 -> 1, 11025 -> 2
constexpr int SAMPLERATE_SHIFT = 1;
constexpr int SAMPLES_PER_TICK = 90 * (SAMPLERATE / 11025);

typedef unsigned char Sample;

constexpr int numNotes = 12;
enum class Note {
    A, As, B, C, Cs, D, Ds, E, F, Fs, G, Gs
};

enum class WaveForm {
    TRIANGLE,
    TILTED_SAW,
    SAW,
    SQUARE,
    PULSE,
    ORGAN
};

enum class Effect {
    NONE,
    SLIDE,
    VIBRATO,
    DROP,
    FADE_IN,
    FADE_OUT,
    ARPEGGIO
};

struct NoteSpec {
    Note note;
    int oct; // [2..6]
    int vol; // [1..8]
    WaveForm wav;
    Effect fx;
};

struct TuneSpec {
    int noteDuration;        // in "ticks". [1..64]
    int loopStart, loopEnd;
    const NoteSpec* notes;
};

struct WaveTable {
    const int numSamples;
    const signed char* samples;
};

class TuneGenerator {
    const TuneSpec* _tuneSpec;
    int _samplesPerNote;

// Current note
    const NoteSpec* _note;
    const WaveTable* _waveTable;
    int _volume, _volumeDelta;
    int _indexDelta, _indexDeltaDelta;
    int _sampleIndex, _endMainIndex;
    int _waveIndex, _maxWaveIndex;
    int _blendSample, _blendDelta;

// Arpeggio effect
    const NoteSpec* _arpeggioNote;

    void inline setSamplesPerNote() {
        _samplesPerNote = _tuneSpec->noteDuration * SAMPLES_PER_TICK;
    }

    void startNote();
    const NoteSpec* peekNextNote() const;
    void moveToNextNote();

    void addMainSamples(Sample* &curP, Sample* endP);
    void addBlendSamples(Sample* &curP, Sample* endP);

public:
    void setTuneSpec(const TuneSpec* tuneSpec);

    // Returns the number of samples added
    int addSamples(Sample* buf, int maxSamples);
};

#endif /* ChannelTune_h */
