//
//  main.cpp
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include <iostream>

#include "Music.h"

#include "WavGeneration.h"

#include "AlexKiddMusic.h"
#include "BubbleBobbleMusic.h"
#include "BumbleBotsMusic.h"
#include "GardenOfLife.h"
#include "NeonMusic.h"
#include "PorklikeMusic.h"
#include "RockForMetalMusic.h"
#include "SweetBunsMusic.h"
#include "TuEdictMusic.h"
#include "TheLairMusic.h"
#include "WintergolfMusic.h"

// One octave in C-Major. TRIANGLE waves.
const TuneSpec testTune1 = TuneSpec {
    .noteDuration = 8,
    .loopStart = 8,
    .numNotes = 8,
    .notes = new NoteSpec[8] {
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::D4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::E4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::F4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::B4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C5, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
    }
};

// Each of the different instruments.
const TuneSpec testTune2 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 6,
    .numNotes = 6,
    .notes = new NoteSpec[6] {
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TILTED_SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::SAW, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::SQUARE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::PULSE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::ORGAN, .fx=Effect::NONE }
   }
};

// Reducing volume
const TuneSpec testTune3 = TuneSpec {
    .noteDuration = 13,
    .loopStart = 8,
    .numNotes = 8,
    .notes = new NoteSpec[8] {
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=3, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=2, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=1, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};

// Volume effects
const TuneSpec testTune4 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 6,
    .numNotes = 6,
    .notes = new NoteSpec[6] {
        NoteSpec { .note=Note::A4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_IN },
        NoteSpec { .note=Note::A4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=4, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_OUT }
    }
};

// Frequency slide - small
const TuneSpec testTune5a = TuneSpec {
    .noteDuration = 32,
    .loopStart = 7,
    .numNotes = 7,
    .notes = new NoteSpec[7] {
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_IN },
        NoteSpec { .note=Note::D4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::E4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::F4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::G4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::B4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE }
    }
};

// Frequency slide - big
const TuneSpec testTune5b = TuneSpec {
    .noteDuration = 32,
    .loopStart = 11,
    .numNotes = 11,
    .notes = new NoteSpec[11] {
        NoteSpec { .note=Note::A1, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_IN },
        NoteSpec { .note=Note::A2, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A5, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A6, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A5, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A2, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A1, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::FADE_OUT }
    }
};

// Arpeggio test
const TuneSpec testTune6a = TuneSpec {
    .noteDuration = 32,
    .loopStart = 16,
    .numNotes = 16,
    .notes = new NoteSpec[16] {
        NoteSpec { .note=Note::C4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::D4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        // Should contain four notes (32 / 8)
        NoteSpec { .note=Note::E4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        // Should contain eight notes (32 / 4)
        NoteSpec { .note=Note::F4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO_FAST },

        // The volume should be the same for all notes part of the same Arpeggio
        // Between subsequent Arpeggios, notes should blend
        NoteSpec { .note=Note::E4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::F4, .vol=7, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::G4, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::A4, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },

        // The wave form should be the same for all notes part of the same Arpeggio.
        // Between subsequent Arpeggios, notes should blend
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::D4, .vol=4, .wav=WaveForm::SAW, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::E4, .vol=8, .wav=WaveForm::ORGAN, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::F4, .vol=4, .wav=WaveForm::PULSE, .fx=Effect::ARPEGGIO },

        // The Arpeggio should still contain four notes but there should only be two Arpeggios
        NoteSpec { .note=Note::G4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::B4, .vol=0, .wav=WaveForm::NONE, .fx=Effect::NONE },
        NoteSpec { .note=Note::C5, .vol=1, .wav=WaveForm::NONE, .fx=Effect::NONE }
    }
};

const TuneSpec testTune6b = TuneSpec {
    .noteDuration = 24, // Strange duration
    .loopStart = 4,
    .numNotes = 4,
    .notes = new NoteSpec[4] {
        // Should contain three notes (24 / 8)
        NoteSpec { .note=Note::C1, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO },
        NoteSpec { .note=Note::C2, .vol=0, .wav=WaveForm::NONE, .fx=Effect::NONE },
        // Should contain six notes (24 / 4)
        NoteSpec { .note=Note::C3, .vol=6, .wav=WaveForm::TRIANGLE, .fx=Effect::ARPEGGIO_FAST },
        NoteSpec { .note=Note::C4, .vol=0, .wav=WaveForm::NONE, .fx=Effect::NONE }
    }
};

// Drop test
const TuneSpec testTune7 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 4,
    .numNotes = 4,
    .notes = new NoteSpec[4] {
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::DROP }
    }
};

// Vibrato test
const TuneSpec testTune8 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 16,
    .numNotes = 16,
    .notes = new NoteSpec[16] {
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::Gs5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::Ds5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds5, .vol=5, .wav=WaveForm::TRIANGLE, .fx=Effect::VIBRATO },
    }
};
const TuneSpec testTune8b = TuneSpec {
    .noteDuration = 32,
    .loopStart = 16,
    .numNotes = 16,
    .notes = new NoteSpec[16] {
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::Gs5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::G5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
        NoteSpec { .note=Note::Ds5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::Ds5, .vol=5, .wav=WaveForm::PHASER, .fx=Effect::VIBRATO },
    }
};

// Noise test
const TuneSpec testTune9 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 10,
    .numNotes = 10,
    .notes = new NoteSpec[10] {
        NoteSpec { .note=Note::A2, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A5, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A6, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A2, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A5, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A6, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
    }
};

// Silence test
const TuneSpec testTune10 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 13,
    .numNotes = 13,
    .notes = new NoteSpec[13] {
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::D4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::E4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::F4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::G4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        SILENCE,
        NoteSpec { .note=Note::B4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};

// Noise test
const TuneSpec testTune11a = TuneSpec {
    .noteDuration = 8,
    .loopStart = 6,
    .numNotes = 6,
    .notes = new NoteSpec[6] {
        NoteSpec { .note=Note::A6, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A5, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A3, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A2, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::SLIDE },
        NoteSpec { .note=Note::A2, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
    }
};
const TuneSpec testTune11b = TuneSpec {
    .noteDuration = 8,
    .loopStart = 7,
    .numNotes = 7,
    .notes = new NoteSpec[7] {
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::D4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::E4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::F4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::G4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
        NoteSpec { .note=Note::B4, .vol=8, .wav=WaveForm::NOISE, .fx=Effect::NONE },
    }
};

// Blend test. It consists of notes with many awkward transitions
const TuneSpec testTune12 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 6,
    .numNotes = 6,
    .notes = new NoteSpec[6] {
        NoteSpec { .note=Note::C4, .vol=2, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        // Big volume jump
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE },
        // Transition to square wave is typically big
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::SQUARE, .fx=Effect::NONE },
        // As it the transition away from it
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::ORGAN, .fx=Effect::NONE },
        // Check transition to silence
        SILENCE,
        // And transition away from it
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::SQUARE, .fx=Effect::NONE },
    }
};

// Silent tune test
const TuneSpec testTune13 = TuneSpec {
    .noteDuration = 16,
    .loopStart = 32,
    .numNotes = 32,
    .notes = nullptr
};

// Test PHASER
const TuneSpec testTune14 = TuneSpec {
    .noteDuration = 32,
    .loopStart = 8,
    .numNotes = 8,
    .notes = new NoteSpec[8] {
        NoteSpec { .note=Note::C4, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::D4, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::E4, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::F4, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::G4, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::A4, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::B4, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
        NoteSpec { .note=Note::C5, .vol=8, .wav=WaveForm::PHASER, .fx=Effect::NONE },
    }
};

int makeSong(const char* filename, const SongSpec* song) {
    int len = song->lengthInSeconds();
    std::cout << "Generating " << filename << ", len = " << len << "s\n";
    makeWav(filename, *song);
    return len;
}

void makeTestTunes() {
    makeWav("test1.wav", testTune1);
    makeWav("test2.wav", testTune2);
    makeWav("test3.wav", testTune3);
    makeWav("test4.wav", testTune4);
    makeWav("test5a.wav", testTune5a);
    makeWav("test5b.wav", testTune5b);
    makeWav("test6a.wav", testTune6a);
    makeWav("test6b.wav", testTune6b);
    makeWav("test7.wav", testTune7);
    makeWav("test8.wav", testTune8);
    makeWav("test8b.wav", testTune8b);
    makeWav("test9.wav", testTune9);
    makeWav("test10.wav", testTune10);
    makeWav("test11a.wav", testTune11a);
    makeWav("test11b.wav", testTune11b);
    makeWav("test12.wav", testTune12);
    makeWav("test13.wav", testTune13);
    makeWav("test14.wav", testTune14);
}

void makeSongs() {
    int totalTime = 0;

    totalTime += makeSong("alex-kidd1.wav", alexKiddSong1);
    totalTime += makeSong("alex-kidd2.wav", alexKiddSong2);
    totalTime += makeSong("alex-kidd3.wav", alexKiddSong3);

    totalTime += makeSong("bb-track1.wav", bumbleBotsSong1);
    totalTime += makeSong("bb-track2.wav", bumbleBotsSong2);

    totalTime += makeSong("bubble-bobble1.wav", bubbleBobbleSong1);
    totalTime += makeSong("bubble-bobble2.wav", bubbleBobbleSong2);

    totalTime += makeSong("garden-of-life.wav", gardenOfLifeSong);

    totalTime += makeSong("neon.wav", neonSong);

    totalTime += makeSong("sweet-buns1.wav", sweetBunsSong1);
    totalTime += makeSong("sweet-buns2.wav", sweetBunsSong2);

    totalTime += makeSong("the-lair1.wav", theLairSong1);
    totalTime += makeSong("the-lair2.wav", theLairSong2);
    totalTime += makeSong("the-lair3.wav", theLairSong3);

    totalTime += makeSong("porklike.wav", porklikeSong);

    totalTime += makeSong("rock-for-metal.wav", rockForMetalSong);

    totalTime += makeSong("tu-edict.wav", technoUtopianEdictSong);

    totalTime += makeSong("wintergolf.wav", wintergolfSong);

    std::cout << "Total time = " << totalTime << "s\n";
}

int main(int argc, const char * argv[]) {
    makeTestTunes();
    makeSongs();

    return 0;
}
