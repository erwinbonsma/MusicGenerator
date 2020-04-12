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

    // Adds samples for the tune to the given buffer. Note, it does not overwrite existing values
    // in the buffer, but adds to the existing value so that multiple generators can contribute to
    // the same buffer. This relies on an overarching orchestrator to clear the buffer values at
    // right moment.
    //
    // Returns the number of samples added. It can less than the maximum when the song ends.
    int addSamples(Sample* buf, int maxSamples);
};

#endif /* SongGenerator_h */
