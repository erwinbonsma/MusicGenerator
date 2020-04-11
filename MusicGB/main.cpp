//
//  main.cpp
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include <iostream>

#include "TuneGenerator.h"
#include "PatternGenerator.h"
#include "SongGenerator.h"

#include "WavGeneration.h"

#include "BumbleBotsMusic.h"

// One octave in C-Major. TRIANGLE waves.
const TuneSpec testTune1 = TuneSpec {
    .noteDuration = 8,
    .loopStart = 8,
    .loopEnd = 8,
    .notes = new NoteSpec[8] {
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::B, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::D, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::E, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::F, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=5, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};

// Each of the different instruments.
const TuneSpec testTune2 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 6,
    .loopEnd = 6,
    .notes = new NoteSpec[6] {
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::SQUARE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::PULSE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::ORGAN, .fx=Effect::NONE }
   }
};

// Reducing volume
const TuneSpec testTune3 = TuneSpec {
    .noteDuration = 13,
    .loopStart = 8,
    .loopEnd = 8,
    .notes = new NoteSpec[8] {
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=3, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=2, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=1, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};

// Volume effects
const TuneSpec testTune4 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 6,
    .loopEnd = 6,
    .notes = new NoteSpec[6] {
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_IN },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_OUT }
    }
};

// Frequency slide - small
const TuneSpec testTune5a = TuneSpec {
    .noteDuration = 32,
    .loopStart = 7,
    .loopEnd = 7,
    .notes = new NoteSpec[7] {
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::B, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::C, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::D, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::E, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::F, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::G, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};

// Frequency slide - big
const TuneSpec testTune5b = TuneSpec {
    .noteDuration = 32,
    .loopStart = 11,
    .loopEnd = 11,
    .notes = new NoteSpec[11] {
        NoteSpec { .note=Note::A, .oct=2, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=5, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=6, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::G, .oct=6, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=6, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=5, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=2, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_OUT }
    }
};

// Arpeggio test
const TuneSpec testTune6 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 16,
    .loopEnd = 16,
    .notes = new NoteSpec[16] {
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::B, .oct=4, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C, .oct=4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::D, .oct=4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },

        NoteSpec { .note=Note::C, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::D, .oct=4, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::E, .oct=4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::F, .oct=4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },

        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::B, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::D, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO_FAST },
        NoteSpec { .note=Note::E, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO_FAST },
        NoteSpec { .note=Note::F, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO_FAST },
        NoteSpec { .note=Note::G, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO_FAST },
        NoteSpec { .note=Note::A, .oct=5, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};

// Drop test
const TuneSpec testTune7 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 4,
    .loopEnd = 4,
    .notes = new NoteSpec[4] {
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP }
    }
};

// Vibrato test
const TuneSpec testTune8 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 16,
    .loopEnd = 16,
    .notes = new NoteSpec[16] {
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::Gs, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::Ds, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
    }
};

// Noise test
const TuneSpec testTune9 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 10,
    .loopEnd = 10,
    .notes = new NoteSpec[10] {
        NoteSpec { .note=Note::A, .oct=2, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=5, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=6, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A, .oct=2, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=3, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=5, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A, .oct=6, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
    }
};

// Silence test
const TuneSpec testTune10 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 13,
    .loopEnd = 13,
    .notes = new NoteSpec[13] {
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::B, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::C, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::D, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::E, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::F, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::G, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};
/*
const TuneSpec sfx16 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 32,
    .loopEnd = 32,
    .notes = new NoteSpec[32] {
        NoteSpec { .note=Note::C,  .oct=4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=3, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=4, .vol=4, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::Ds, .oct=4, .vol=1, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        SILENCE,

        NoteSpec { .note=Note::C,  .oct=4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=3, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=4, .vol=4, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::Ds, .oct=4, .vol=1, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        SILENCE,

        NoteSpec { .note=Note::C,  .oct=4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=3, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=4, .vol=4, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },

        NoteSpec { .note=Note::C,  .oct=4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=4, .vol=5, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::Ds, .oct=4, .vol=1, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        SILENCE
    }
};

const TuneSpec sfx24 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 32,
    .loopEnd = 32,
    .notes = new NoteSpec[32] {
        NoteSpec { .note=Note::G,  .oct=4, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::D,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=3, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },

        NoteSpec { .note=Note::G,  .oct=4, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::D,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::Gs, .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },

        NoteSpec { .note=Note::G,  .oct=4, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::D,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=3, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },

        NoteSpec { .note=Note::G,  .oct=4, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=1, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds, .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::D,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::C,  .oct=5, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::Gs, .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::G,  .oct=4, .vol=3, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE }
    }
};

const TuneSpec sfx26 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 32,
    .loopEnd = 32,
    .notes = new NoteSpec[32] {
        NoteSpec { .note=Note::C,  .oct=3, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=4, .vol=5, .wav=WaveForm::NOISE, .fx=Effect::FADE_OUT },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },

        NoteSpec { .note=Note::C,  .oct=3, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=4, .vol=5, .wav=WaveForm::NOISE, .fx=Effect::FADE_OUT },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::NOISE, .fx=Effect::FADE_OUT },
        NoteSpec { .note=Note::C,  .oct=3, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::C,  .oct=2, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },

        NoteSpec { .note=Note::C,  .oct=3, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=4, .vol=5, .wav=WaveForm::NOISE, .fx=Effect::FADE_OUT },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },

        NoteSpec { .note=Note::C,  .oct=3, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        SILENCE,
        NoteSpec { .note=Note::C,  .oct=4, .vol=5, .wav=WaveForm::NOISE, .fx=Effect::FADE_OUT },
        NoteSpec { .note=Note::C,  .oct=3, .vol=5, .wav=WaveForm::NOISE, .fx=Effect::FADE_OUT },
        NoteSpec { .note=Note::C,  .oct=3, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::C,  .oct=2, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP }
    }
};
*/

int main(int argc, const char * argv[]) {
    /*
    makeWav("test1.wav", testTune1);
    makeWav("test2.wav", testTune2);
    makeWav("test3.wav", testTune3);
    makeWav("test4.wav", testTune4);
    makeWav("test5a.wav", testTune5a);
    makeWav("test5b.wav", testTune5b);
    makeWav("test6.wav", testTune6);
    makeWav("test7.wav", testTune7);
    makeWav("test8.wav", testTune8);
    makeWav("test9.wav", testTune9);
    makeWav("test10.wav", testTune10);

    makeWav("sfx16.wav", sfx16);
    makeWav("sfx24.wav", sfx24);
    makeWav("sfx26.wav", sfx26);
    */

    makeWav("bb-track1a.wav", bumbleBotsSong, false);
    makeWav("bb-track1b.wav", bumbleBotsSong, true);

    return 0;
}
