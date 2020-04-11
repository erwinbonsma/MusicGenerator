//
//  PatternGenerator.cpp
//  MusicGB
//
//  Created by Erwin on 11/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include "PatternGenerator.h"

void PatternGenerator::setPatternSpec(const PatternSpec* patternSpec) {
    _patternSpec = patternSpec;
    for (int i = 0; i < _patternSpec->numTunes; i++) {
        _tuneGens[i].setTuneSpec(_patternSpec->tunes[i]);
    }
}

int PatternGenerator::addSamples(Sample** buf, int maxSamples) {
    int numSamples = maxSamples;

    for (int i = 0; i < _patternSpec->numTunes; i++) {
        numSamples = _tuneGens[i].addSamples(buf[i], numSamples);
    }
    for (int i = _patternSpec->numTunes; i < MAX_TUNES; i++) {
        for (int j = 0; j < numSamples; j++) {
            buf[i][j] = 128;
        }
    }

    return numSamples;
}
