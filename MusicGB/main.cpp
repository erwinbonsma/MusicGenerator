//
//  main.cpp
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include <iostream>

#include "ChannelTune.h"

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
        NoteSpec { .note=Note::A, .oct=4, .vol=8, .wav=WaveForm::TRIANGLE, .fx=Effect::NONE }
    }
};


int main(int argc, const char * argv[]) {
    TuneGenerator tuneGen;

    tuneGen.setTuneSpec(&testTune);
    int buf[512];
    int samplesAdded;
    do {
        samplesAdded = tuneGen.addSamples(buf, 512);
        for (int i = 0; i < samplesAdded; i++) {
            std::cout << buf[i] << "\n";
        }
    } while (0 && samplesAdded == 512);

    return 0;
}
