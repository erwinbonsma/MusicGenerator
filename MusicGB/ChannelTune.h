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

constexpr int sampleRate = 22050;
constexpr int samplesPerTick = 90 * (sampleRate / 11025);

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
    FADE_OUT
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
    const int* samples;
};

class TuneGenerator {
    const TuneSpec* _tuneSpec;
    int _samplesPerNote;
    const NoteSpec* _note;
    const NoteSpec* _lastNote;
    const NoteSpec* _resumeNote;

// Current note
    const WaveTable* _waveTable;
    int _volumeStart, _volumeEnd;
    int _indexDeltaStart, _indexDeltaEnd;
    int _sampleIndex, _endMainIndex;
    int _waveIndex, _maxWaveIndex;

    void startNote();
    const NoteSpec* nextNote() const;

    void addMainSamples(Sample* &curP, Sample* endP);
    void addRampDownSamples(Sample* &curP, Sample* endP);

public:
    void setTuneSpec(const TuneSpec* tuneSpec);

    // Returns the number of samples added
    int addSamples(Sample* buf, int maxSamples);
};

#endif /* ChannelTune_h */
