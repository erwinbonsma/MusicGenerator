//
//  main.cpp
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include <iostream>

#include "ChannelTune.h"
#include "wav.h"

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


void makeWav(const char* filename, const TuneSpec& tune) {
    TuneGenerator tuneGen;

    tuneGen.setTuneSpec(&tune);
    Sample buf[512];
    Sample* buffers[1] = { buf };
    int samplesAdded;
    WavFile* wavFile = wav_open(filename, "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_num_channels(wavFile, 1);
    wav_set_sample_rate(wavFile, sampleRate);
    wav_set_sample_size(wavFile, 1);

    do {
        samplesAdded = tuneGen.addSamples(buf, 512);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == 512);

    wav_close(wavFile);
}

int main(int argc, const char * argv[]) {
    makeWav("test1.wav", testTune1);
    makeWav("test2.wav", testTune2);
    makeWav("test3.wav", testTune3);

    return 0;
}
