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

void makeWav(const char* filename, const TuneSpec& tune) {
    TuneGenerator tuneGen;

    tuneGen.setTuneSpec(&tune);
    Sample buf[BUFSIZE];
    Sample* buffers[1] = { buf };

    WavFile* wavFile = wav_open(filename, "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_num_channels(wavFile, 1);
    wav_set_sample_rate(wavFile, SAMPLERATE);
    wav_set_sample_size(wavFile, 1);

    int samplesAdded;
    do {
        samplesAdded = tuneGen.addSamples(buf, BUFSIZE);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}

void makeWav(const char* filename, const PatternSpec& pattern) {
    PatternGenerator patternGen;

    patternGen.setPatternSpec(&pattern);
    Sample buf[BUFSIZE * MAX_TUNES];
    Sample* buffers[4] = { buf, buf + BUFSIZE, buf + BUFSIZE * 2, buf + BUFSIZE * 3 };

    WavFile* wavFile = wav_open(filename, "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_num_channels(wavFile, pattern.numTunes);
    wav_set_sample_rate(wavFile, SAMPLERATE);
    wav_set_sample_size(wavFile, 1);

    int samplesAdded;
    do {
        samplesAdded = patternGen.addSamples(buffers, BUFSIZE);
        wav_write(wavFile, (const void* const*)buffers, samplesAdded);
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}

void makeWav(const char* filename, const SongSpec& song, bool singleTrack) {
    SongGenerator songGenerator;

    songGenerator.setSongSpec(&song);
    Sample buf[BUFSIZE * MAX_TUNES];
    Sample* buffers[4] = { buf, buf + BUFSIZE, buf + BUFSIZE * 2, buf + BUFSIZE * 3 };
    int16_t oneTrackBuf[BUFSIZE];
    int16_t* oneTrackBuffers[1] = { oneTrackBuf };

    WavFile* wavFile = wav_open(filename, "w");
    wav_set_format(wavFile, WAV_FORMAT_PCM);
    wav_set_sample_rate(wavFile, SAMPLERATE);
    if (singleTrack) {
        wav_set_num_channels(wavFile, 1);
        wav_set_sample_size(wavFile, 2);
    } else {
        wav_set_num_channels(wavFile, 4);
        wav_set_sample_size(wavFile, 1);
    }

    int samplesAdded;
    do {
        samplesAdded = songGenerator.addSamples(buffers, BUFSIZE);
        if (singleTrack) {
            for (int i = samplesAdded; --i >= 0; ) {
                int sum = 0;
                for (int j = 4; --j >= 0; ) {
                    sum += ((int)buffers[j][i]) - 128;
                }
                // Scale to full range and make unsigned
                oneTrackBuf[i] = (int16_t)((sum << 6) + 0 * 32768);
            }
            wav_write(wavFile, (const void* const*)oneTrackBuffers, samplesAdded);
        } else {
            wav_write(wavFile, (const void* const*)buffers, samplesAdded);
        }
    } while (samplesAdded == BUFSIZE);

    wav_close(wavFile);
}
