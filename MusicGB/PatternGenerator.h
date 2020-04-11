//
//  PatternGenerator.h
//  MusicGB
//
//  Created by Erwin on 11/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#ifndef PatternGenerator_h
#define PatternGenerator_h

#include "TuneGenerator.h"

struct PatternSpec {
    int numTunes;
    const TuneSpec** tunes;
};

constexpr int MAX_TUNES = 4;

class PatternGenerator {
    const PatternSpec* _patternSpec;
    TuneGenerator _tuneGens[MAX_TUNES];

public:
    void setPatternSpec(const PatternSpec* patternSpec);

    // Returns the number of samples added
    int addSamples(Sample** buf, int maxSamples);
};

#endif /* PatternGenerator_h */
