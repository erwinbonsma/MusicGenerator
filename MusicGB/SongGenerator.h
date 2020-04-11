//
//  SongGenerator.h
//  MusicGB
//
//  Created by Erwin on 11/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#ifndef SongGenerator_h
#define SongGenerator_h

#include "PatternGenerator.h"

struct SongSpec {
    int numPatterns;
    const PatternSpec** patterns;
};

class SongGenerator {
    const SongSpec* _songSpec;
    PatternGenerator _patternGenerator;
    const PatternSpec** _pattern;

    void startPattern();
    void moveToNextPattern();

public:
    void setSongSpec(const SongSpec* songSpec);

    // Returns the number of samples added
    int addSamples(Sample** buf, int maxSamples);
};

#endif /* SongGenerator_h */
