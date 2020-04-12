//
//  TuneGenerator.h
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#ifndef TuneGenerator_h
#define TuneGenerator_h

#include <stdint.h>

constexpr int SAMPLERATE = 22050;
// Should be updated when changing SAMPLERATE as follows:
// 44100 -> 0, 22050 -> 1, 11025 -> 2
constexpr int SAMPLERATE_SHIFT = 1;
constexpr int SAMPLES_PER_TICK = 90 * (SAMPLERATE / 11025);

typedef int16_t Sample;

constexpr int numNotes = 12;
enum class Note {
    A, As, B, C, Cs, D, Ds, E, F, Fs, G, Gs
};

enum class WaveForm : int8_t {
    TRIANGLE,
    TILTED_SAW,
    SAW,
    SQUARE,
    PULSE,
    ORGAN,
    NOISE,
    PHASER,
    NONE
};

enum class Effect : int8_t {
    NONE,
    SLIDE,
    VIBRATO,
    DROP,
    FADE_IN,
    FADE_OUT,
    ARPEGGIO,
    ARPEGGIO_FAST
};

struct NoteSpec {
    Note note;
    uint8_t oct; // [2..6]
    uint8_t vol; // [0..8]
    WaveForm wav;
    Effect fx;
};

const NoteSpec SILENCE = NoteSpec {
    .note = Note::A, .oct = 4, .vol = 0, .wav = WaveForm::NONE, .fx = Effect::NONE
};

struct TuneSpec {
    uint8_t noteDuration;        // in "ticks". [1..64]
    uint8_t loopStart, loopEnd;
    const NoteSpec* notes;
};

struct WaveTable {
    const uint16_t numSamples;
    const int8_t* samples;
};

class TuneGenerator {
    const TuneSpec* _tuneSpec;
    uint16_t _samplesPerNote;

// Current note
    const NoteSpec* _note;
    const WaveTable* _waveTable;
    const NoteSpec* _arpeggioNote;
    uint16_t _sampleIndex, _endMainIndex;
    int16_t _volume, _volumeDelta;
    int16_t _blendSample, _blendDelta;
    int32_t _waveIndex, _maxWaveIndex;
    int _indexDelta, _indexDeltaDelta;
    int _vibratoDelta, _vibratoDeltaDelta;
    int _noiseShift;

    void inline setSamplesPerNote() {
        _samplesPerNote = _tuneSpec->noteDuration * SAMPLES_PER_TICK;
    }

    int inline arpeggioFactor(const NoteSpec* note) const {
        // Assumes effect is ARPEGGIO or ARPEGGIO_FAST
        return (note->fx == Effect::ARPEGGIO_FAST ? 3 : 2) - (_tuneSpec->noteDuration <= 8 ? 1 : 0);
    }
    bool isFirstArpeggioNote() const;
    bool isLastArpeggioNote() const;

    void startNote();

    const NoteSpec* peekPrevNote() const;
    const NoteSpec* peekNextNote() const;
    void moveToNextNote();

    void addMainSamples(Sample* &curP, Sample* endP);
    void addMainSamplesSilence(Sample* &curP, Sample* endP);
    void addMainSamplesVibrato(Sample* &curP, Sample* endP);
    void addBlendSamples(Sample* &curP, Sample* endP);

public:
    void setTuneSpec(const TuneSpec* tuneSpec);

    // Adds samples for the tune to the given buffer. Note, it does not overwrite existing values
    // in the buffer, but adds to the existing value so that multiple generators can contribute to
    // the same buffer. This relies on an overarching orchestrator to clear the buffer values at
    // right moment.
    //
    // Returns the number of samples added. It can less than the maximum when the tune ends.
    int addSamples(Sample* buf, int maxSamples);
};

#endif /* TuneGenerator_h */
