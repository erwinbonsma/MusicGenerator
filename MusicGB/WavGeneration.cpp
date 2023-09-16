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
#include <iostream>
#include <algorithm>

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

WavFile* openWavFile(const char* filename) {
    WavFile* wavFile = wav_open(filename, "w");

    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_num_channels(wavFile, 1);
    wav_set_sample_rate(wavFile, SAMPLERATE);
    wav_set_sample_size(wavFile, 2);

    return wavFile;
}

void makeWav(const char* filename, const TuneSpec& tune, int maxSamples) {
    TuneGenerator tuneGen = TuneGenerator();

    tuneGen.setTuneSpec(&tune);
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = openWavFile(filename);
    size_t samplesAdded;
    do {
        clearBuffer(buf);
        Sample* maxP = buf + std::min(BUFSIZE, maxSamples);
        samplesAdded = tuneGen.addSamples(buf, maxP) - buf;
        maxSamples -= samplesAdded;
        amplifyBuffer(buf, 8);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}

void makeWav(const char* filename, const PatternSpec& pattern) {
    PatternGenerator patternGen = PatternGenerator();

    patternGen.setPatternSpec(&pattern);
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = openWavFile(filename);
    size_t samplesAdded;
    do {
        clearBuffer(buf);
        samplesAdded = patternGen.addSamples(buf, buf + BUFSIZE) - buf;
        amplifyBuffer(buf, 6);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}

void makeWav(const char* filename, const SongSpec& song) {
    SongGenerator songGenerator = SongGenerator();

    songGenerator.setSongSpec(&song, false);
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = openWavFile(filename);
    size_t samplesAdded;
    do {
        clearBuffer(buf);
        samplesAdded = songGenerator.addSamples(buf, buf + BUFSIZE) - buf;
        amplifyBuffer(buf, 6);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}

void makeWav(const char* filename, MusicHandler& musicHandler, bool outputLevel) {
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = openWavFile(filename);
    do {
        musicHandler.update();
        if (outputLevel) {
            std::cout << std::string(musicHandler.outputLevel(), '#') << std::endl;
        }
        for (int i = 0; i < BUFSIZE; i++) {
            buf[i] = musicHandler.nextSample() << 6;
        }
        wav_write(wavFile, (const void* const*)buffers, BUFSIZE);
    } while (musicHandler.isPlaying());

    wav_close(wavFile);
}
