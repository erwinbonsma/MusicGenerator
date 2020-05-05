//
//  Music.h
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#ifndef Music_h
#define Music_h

#include <stdint.h>

#ifdef STANDALONE
  #define SOUND_MUSIC_BUFFERSIZE 1024
  #define OPTIMIZE_ATTRIBUTE
#else
  #include "../../config/config.h"
  #define OPTIMIZE_ATTRIBUTE __attribute__((optimize("-O3")))
  // Undefine (silly) defines from binary.h which interfere with Note enumeration type
  #ifdef B0
    #undef B0
  #endif
  #ifdef B1
    #undef B1
  #endif
  namespace Gamebuino_Meta {
#endif

//--------------------------------------------------------------------------------------------------
// Tune Generation

#ifdef STANDALONE
  constexpr uint16_t SAMPLERATE = 11025;

  // Should be updated when changing SAMPLERATE as follows:
  // 44100 -> 2, 22050 -> 1, 11025 -> 0
  constexpr uint8_t SAMPLERATE_SHIFT = 0;
#else
  constexpr uint16_t SAMPLERATE = SOUND_FREQ;
  #if SOUND_FREQ==44100
    constexpr uint8_t SAMPLERATE_SHIFT = 2;
  #elif SOUND_FREQ==22050
    constexpr uint8_t SAMPLERATE_SHIFT = 1;
  #else
    constexpr uint8_t SAMPLERATE_SHIFT = 0;
  #endif
#endif

// Number of samples when sample rate is 11025 Hz
constexpr uint8_t SAMPLES_PER_TICK = 90;

constexpr uint8_t BLEND_SHIFT = 4;
constexpr uint8_t NUM_BLEND_SAMPLES = 1 << BLEND_SHIFT;

typedef int16_t Sample;

constexpr int numNotes = 12;
enum class Note : int8_t {
    C0  = 0x00, Cs0 = 0x01, D0  = 0x02, Ds0 = 0x03, E0  = 0x04, F0  = 0x05,
    Fs0 = 0x06, G0  = 0x07, Gs0 = 0x08, A0  = 0x09, As0 = 0x0a, B0  = 0x0b,
    C1  = 0x10, Cs1 = 0x11, D1  = 0x12, Ds1 = 0x13, E1  = 0x14, F1  = 0x15,
    Fs1 = 0x16, G1  = 0x17, Gs1 = 0x18, A1  = 0x19, As1 = 0x1a, B1  = 0x1b,
    C2  = 0x20, Cs2 = 0x21, D2  = 0x22, Ds2 = 0x23, E2  = 0x24, F2  = 0x25,
    Fs2 = 0x26, G2  = 0x27, Gs2 = 0x28, A2  = 0x29, As2 = 0x2a, B2  = 0x2b,
    C3  = 0x30, Cs3 = 0x31, D3  = 0x32, Ds3 = 0x33, E3  = 0x34, F3  = 0x35,
    Fs3 = 0x36, G3  = 0x37, Gs3 = 0x38, A3  = 0x39, As3 = 0x3a, B3  = 0x3b,
    C4  = 0x40, Cs4 = 0x41, D4  = 0x42, Ds4 = 0x43, E4  = 0x44, F4  = 0x45,
    Fs4 = 0x46, G4  = 0x47, Gs4 = 0x48, A4  = 0x49, As4 = 0x4a, B4  = 0x4b,
    C5  = 0x50, Cs5 = 0x51, D5  = 0x52, Ds5 = 0x53, E5  = 0x54, F5  = 0x55,
    Fs5 = 0x56, G5  = 0x57, Gs5 = 0x58, A5  = 0x59, As5 = 0x5a, B5  = 0x5b,
    C6  = 0x60, Cs6 = 0x61, D6  = 0x62, Ds6 = 0x63, E6  = 0x64, F6  = 0x65,
    Fs6 = 0x66, G6  = 0x67, Gs6 = 0x68, A6  = 0x69, As6 = 0x6a, B6  = 0x6b,
    C7  = 0x70, Cs7 = 0x71, D7  = 0x72, Ds7 = 0x73, E7  = 0x74, F7  = 0x75,
    Fs7 = 0x76, G7  = 0x77, Gs7 = 0x78, A7  = 0x79, As7 = 0x7a, B7  = 0x7b
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
    uint8_t vol; // [0..8]
    WaveForm wav;
    Effect fx;
};

#define SILENCE NoteSpec { .note = Note::C4, .vol = 0, .wav = WaveForm::NONE, .fx = Effect::NONE }

struct TuneSpec {
    uint8_t noteDuration;        // in "ticks". [1..64]
    uint16_t loopStart, numNotes;
    const NoteSpec *const notes;

    int lengthInTicks() const;
};

struct WaveTable {
    const uint16_t numSamples;
    // Used to let waves start at zero (where possible). It is (currently) only needed for PHASER,
    // as this waveform is dynamically calculated, and in the most efficient implementation the
    // waves do not have zero amplitude when the wave index is zero.
    const uint16_t initialWaveIndex;
    const uint8_t shift;
    const int8_t *const samples;
};

class TuneGenerator;
typedef void (TuneGenerator::*SampleGeneratorFun)(Sample* &curP, Sample* endP);

class TuneGenerator {
    const TuneSpec* _tuneSpec;
    int16_t _samplesPerNote;
    int16_t _noiseLfsr = 1;

// Current note
    const NoteSpec* _note;
    const NoteSpec* _arpeggioNote;
    const WaveTable* _waveTable;
    int32_t _waveIndex, _maxWaveIndex;
    int32_t _indexDelta, _indexDeltaDelta;
    int32_t _volume, _volumeDelta;
    int32_t _vibratoDelta, _vibratoDeltaDelta;
    int32_t _indexNoiseDelta;
    int16_t _sampleIndex;
    int16_t _pendingArpeggioSamples;
    int16_t _noteIndex; // Used when tune contains only SILENCE
    uint8_t _phaserCount;

    SampleGeneratorFun _sampleGeneratorFun;

// Blending
    Sample _blendBufOut[NUM_BLEND_SAMPLES], _blendBufIn[NUM_BLEND_SAMPLES];
    uint8_t _numBlendSamples;

    void inline setSamplesPerNote() {
        _samplesPerNote = (_tuneSpec->noteDuration * SAMPLES_PER_TICK) << SAMPLERATE_SHIFT;
    }

    const inline NoteSpec* firstArpeggioNote() const {
        return _arpeggioNote - (_arpeggioNote - _tuneSpec->notes) % 4;
    }
    const inline NoteSpec* lastArpeggioNote() const {
        return _arpeggioNote - (_arpeggioNote - _tuneSpec->notes) % 4 + 3;
    }

    // Returns true when an actual wave has been set. False when the wave form was NONE.
    bool setWaveTable(WaveForm waveForm);

    void startArpeggio();
    void exitArpeggio();

    void startNote();

    const NoteSpec* peekPrevNote() const;
    const NoteSpec* peekNextNote() const;
    void moveToNextNote();

    void createOutgoingBlendSamplesIfNeeded();
    void createIncomingBlendSamplesIfNeeded();

    void addMainSamples(Sample* &curP, Sample* endP) OPTIMIZE_ATTRIBUTE;
    void addMainSamplesPhaser(Sample* &curP, Sample* endP) OPTIMIZE_ATTRIBUTE;
    void addMainSamplesNoise(Sample* &curP, Sample* endP) OPTIMIZE_ATTRIBUTE;
    void addMainSamplesSilence(Sample* &curP, Sample* endP) OPTIMIZE_ATTRIBUTE;
    void addMainSamplesVibrato(Sample* &curP, Sample* endP) OPTIMIZE_ATTRIBUTE;
    void addBlendSamples(Sample* &curP, Sample* endP) OPTIMIZE_ATTRIBUTE;

public:
    void setTuneSpec(const TuneSpec* tuneSpec, bool isFirst = true);
    void stop() { _note = nullptr; }
    bool isDone() { return _note == nullptr; }

    int ticksPlayed();
    int outputLevel();

    // Adds samples for the tune to the given buffer. Note, it does not overwrite existing values
    // in the buffer, but adds to the existing value so that multiple generators can contribute to
    // the same buffer. This relies on an overarching orchestrator to clear the buffer values at
    // right moment.
    //
    // Note: it should only be invoked when isDone() returns false.
    //
    // Returns the number of samples added. It can less than the maximum when the tune ends.
    int addSamples(Sample* buf, int maxSamples);
};

//--------------------------------------------------------------------------------------------------
// Pattern Generation

struct PatternSpec {
    uint8_t numTunes;
    const TuneSpec *const *const tunes;

    int lengthInTicks() const;
};

constexpr int MAX_TUNES_IN_PATTERN = 4;

class PatternGenerator {
    const PatternSpec* _patternSpec;
    TuneGenerator _tuneGens[MAX_TUNES_IN_PATTERN];

public:
    void setPatternSpec(const PatternSpec* patternSpec, bool isFirst = true);

    int ticksPlayed();
    int outputLevel();

    // Adds samples for the pattern to the given buffer. Note, it does not overwrite existing values
    // in the buffer, but adds to the existing value so that multiple generators can contribute to
    // the same buffer. This relies on an overarching orchestrator to clear the buffer values at
    // right moment.
    //
    // Returns the number of samples added. It can less than the maximum when the pattern ends.
    int addSamples(Sample* buf, int maxSamples);
};

//--------------------------------------------------------------------------------------------------
// Song Generation

struct SongSpec {
    uint8_t loopStart, numPatterns;
    const PatternSpec *const *const patterns;

    int lengthInSeconds() const;
    int lengthInTicks() const;
};

class SongGenerator {
    const SongSpec* _songSpec;
    PatternGenerator _patternGenerator;
    const PatternSpec *const * _pattern;
    int _ticksPlayedInEarlierPatterns;
    bool _loop;
    bool _paused;

    void startPattern(bool isFirst);
    void moveToNextPattern();

public:
    void setSongSpec(const SongSpec* songSpec, bool loop);
    void stop() { _pattern = nullptr; };

    void setLoop(bool flag) { _loop = flag; }
    void setPause(bool flag) { _paused = flag; }

    bool isLooping() { return _loop; }
    bool isPaused() { return _paused; }
    int progressInSeconds();
    bool isDone() { return _pattern == nullptr; }

    int outputLevel();

    // Adds samples for the tune to the given buffer. Note, it does not overwrite existing values
    // in the buffer, but adds to the existing value so that multiple generators can contribute to
    // the same buffer. This relies on an overarching orchestrator to clear the buffer values at
    // right moment.
    //
    // Note: it should only be invoked when isDone() returns false.
    //
    // Returns the number of samples added. It can less than the maximum when the song ends.
    int addSamples(Sample* buf, int maxSamples);
};

//--------------------------------------------------------------------------------------------------
// Music Handler

class MusicHandler {
    int16_t _buffer[SOUND_MUSIC_BUFFERSIZE];
    int16_t* _readP;
    int16_t* _headP;
    int16_t* _endP;
    int16_t* _zeroP;

    TuneGenerator _tuneGenerator;
    SongGenerator _songGenerator;

public:
    MusicHandler();

    void play(const TuneSpec* tuneSpec);
    void play(const SongSpec* songSpec, bool loop);

    bool isTunePlaying() { return !_tuneGenerator.isDone(); }

    bool isSongPlaying() { return !(_songGenerator.isDone() || _songGenerator.isPaused()); }
    bool isSongPaused() { return _songGenerator.isPaused(); }
    bool isSongLooping() { return _songGenerator.isLooping(); }

    bool isPlaying() { return isTunePlaying() || isSongPlaying(); }

    void stopTune() { _tuneGenerator.stop(); }
    void stopSong() { _songGenerator.stop(); }

    void loopSong(bool flag) { _songGenerator.setLoop(flag); }
    void pauseSong(bool flag) { _songGenerator.setPause(flag); }
    int songProgressInSeconds() { return _songGenerator.progressInSeconds(); }

    int outputLevel();

    void update();

    uint16_t nextSample() {
        uint16_t sample = *_readP;
        if (++_readP == _endP) { _readP = _buffer; }
        return sample;
    }
};

#ifndef STANDALONE
} // Namespace
#endif

#endif /* Music_h */
