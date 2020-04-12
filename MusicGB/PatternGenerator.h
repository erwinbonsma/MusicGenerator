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
    uint8_t numTunes;
    const TuneSpec** tunes;
};

constexpr int MAX_TUNES = 4;

class PatternGenerator {
    const PatternSpec* _patternSpec;
    TuneGenerator _tuneGens[MAX_TUNES];

public:
    void setPatternSpec(const PatternSpec* patternSpec);

    // Adds samples for the pattern to the given buffer. Note, it does not overwrite existing values
    // in the buffer, but adds to the existing value so that multiple generators can contribute to
    // the same buffer. This relies on an overarching orchestrator to clear the buffer values at
    // right moment.
    //
    // Returns the number of samples added. It can less than the maximum when the pattern ends.
    int addSamples(Sample* buf, int maxSamples);
};

#endif /* PatternGenerator_h */
