//
//  WavGeneration.h
//  MusicGB
//
//  Created by Erwin on 11/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#ifndef WavGeneration_h
#define WavGeneration_h

#include "TuneGenerator.h"
#include "PatternGenerator.h"
#include "SongGenerator.h"

void makeWav(const char* filename, const TuneSpec& tune);
void makeWav(const char* filename, const PatternSpec& pattern);
void makeWav(const char* filename, const SongSpec& song);

#endif /* WavGeneration_h */
