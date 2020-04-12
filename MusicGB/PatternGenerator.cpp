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

int PatternGenerator::addSamples(Sample* buf, int maxSamples) {
    // The first tune detemines the length of the pattern. It should therefore not loop.
    int numSamples = _tuneGens[0].addSamples(buf, maxSamples);

    for (int i = 1; i < _patternSpec->numTunes; i++) {
        _tuneGens[i].addSamples(buf, numSamples);
    }

    return numSamples;
}
