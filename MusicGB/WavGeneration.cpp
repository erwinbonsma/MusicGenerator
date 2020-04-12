//
//  WavGeneration.cpp
//  MusicGB
//
//  Created by Erwin on 11/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include "WavGeneration.h"

#include "wav.h"
#include <stdint.h>

constexpr int BUFSIZE = 512;

void clearBuffer(Sample* buf) {
    Sample* bufP = buf + BUFSIZE;
    do {
        *--bufP = 0;
    } while (bufP != buf);
}

void amplifyBuffer(Sample* buf, int shift) {
    Sample* bufP = buf + BUFSIZE;
    do {
        *--bufP <<= shift;
    } while (bufP != buf);
}

void makeWav(const char* filename, const TuneSpec& tune) {
    TuneGenerator tuneGen;

    tuneGen.setTuneSpec(&tune);
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = wav_open(filename, "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_num_channels(wavFile, 1);
    wav_set_sample_rate(wavFile, SAMPLERATE);
    wav_set_sample_size(wavFile, 2);

    int samplesAdded;
    do {
        clearBuffer(buf);
        samplesAdded = tuneGen.addSamples(buf, BUFSIZE);
        amplifyBuffer(buf, 8);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}

void makeWav(const char* filename, const PatternSpec& pattern) {
    PatternGenerator patternGen;

    patternGen.setPatternSpec(&pattern);
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = wav_open(filename, "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_num_channels(wavFile, 1);
    wav_set_sample_rate(wavFile, SAMPLERATE);
    wav_set_sample_size(wavFile, 2);

    int samplesAdded;
    do {
        clearBuffer(buf);
        samplesAdded = patternGen.addSamples(buf, BUFSIZE);
        amplifyBuffer(buf, 6);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}

void makeWav(const char* filename, const SongSpec& song) {
    SongGenerator songGenerator;

    songGenerator.setSongSpec(&song);
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = wav_open(filename, "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_sample_rate(wavFile, SAMPLERATE);
    wav_set_num_channels(wavFile, 1);
    wav_set_sample_size(wavFile, 2);

    int samplesAdded;
    do {
        clearBuffer(buf);
        samplesAdded = songGenerator.addSamples(buf, BUFSIZE);
        amplifyBuffer(buf, 6);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}
