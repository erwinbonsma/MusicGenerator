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

const TuneSpec testTune = TuneSpec {
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


int main(int argc, const char * argv[]) {
    TuneGenerator tuneGen;

    tuneGen.setTuneSpec(&testTune);
    Sample buf[512];
    Sample* buffers[1] = { buf };
    int samplesAdded;
    WavFile* wavFile = wav_open("test.wav", "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_num_channels(wavFile, 1);
    wav_set_sample_rate(wavFile, sampleRate);
    wav_set_sample_size(wavFile, 1);

    do {
        samplesAdded = tuneGen.addSamples(buf, 512);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == 512);

    wav_close(wavFile);

    return 0;
}
