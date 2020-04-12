//
//  SongGenerator.cpp
//  MusicGB
//
//  Created by Erwin on 11/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include "SongGenerator.h"

#include <iostream>

void SongGenerator::startPattern() {
    _patternGenerator.setPatternSpec(*_pattern);
}

void SongGenerator::moveToNextPattern() {
    _pattern++;
    if (_pattern == _songSpec->patterns + _songSpec->numPatterns) {
        _pattern = nullptr;
    }
}

void SongGenerator::setSongSpec(const SongSpec* songSpec) {
    _songSpec = songSpec;
    _pattern = _songSpec->patterns;
    startPattern();
}

int SongGenerator::addSamples(Sample* buf, int maxSamples) {
    int totalAdded = 0;

    do {
        totalAdded += _patternGenerator.addSamples(buf, maxSamples - totalAdded);
        if (totalAdded < maxSamples) {
            if (_pattern == nullptr) {
                // We're done
                return totalAdded;
            }
            moveToNextPattern();
            if (_pattern == nullptr) {
                // We're done
                return totalAdded;
            }
            startPattern();
        }
    } while (totalAdded < maxSamples);

    return totalAdded;
}
