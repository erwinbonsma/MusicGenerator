//
//  WavGeneration.h
//  MusicGB
//
//  Created by Erwin on 11/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#ifndef WavGeneration_h
#define WavGeneration_h

#include "Music.h"

void makeWav(const char* filename, const TuneSpec& tune);
void makeWav(const char* filename, const PatternSpec& pattern);
void makeWav(const char* filename, const SongSpec& song);

void makeWav(const char* filename, MusicHandler& musicHandler, bool outputIntensity = false);

#endif /* WavGeneration_h */
