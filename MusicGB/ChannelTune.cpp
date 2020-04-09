//
//  ChannelTune.cpp
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include "ChannelTune.h"

#include <algorithm>
#include <iostream>

constexpr int WAVETABLE_SHIFT = 15;

constexpr int NUM_RAND_BITS = 31; // Update based on RAND_MAX

constexpr int PERIOD_SHIFT = 6;

// Volume can span three bytes. This way, it will not overflow when multiplied with the wave table,
// yet it has the highest resolution for volume-slide and fade effects.
constexpr int VOLUME_SHIFT = 21;

// Controls the amount of frequency change.
constexpr int VIBRATO_MAGNITUDE_SHIFT = 7;

// The amount of wave periods spanned by a single period of the Vibrato effect.
constexpr int VIBRATO_META_PERIOD = 24;

// The volume from the note spec uses up three bits. Note, even though the maximum value is eight,
// it fits in three bits as one is subtracted after applying the volume shift. So:
// vol = 8 => [111] [111....111]
// vol = 6 => [101] [111....111]
// vol = 1 => [000] [111....111]
//              ^         ^
//              |         +--- VOLUME_SHIFT ones
//              +------------- VOLUME_BITs
// Note: Zero volume should not be used for normal notes. Use SILENCE instead (TODO).
constexpr int VOLUME_BITS = 3;

// Notes:
// - The size of wave tables varies to match the wave form to avoid rounding-artifacts and to
//   efficiently capture each. This is compensated for during sample generation.
// - Where possible, wave forms start at 0x80 (the zero position) so that there's no need to ramp
//   up after a change of instrument.
const WaveTable triangleWave = WaveTable {
    .numSamples = 510,
    .samples = new signed char[510] {
           0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
          16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
          32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
          48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
          64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
          80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
          96,   97,   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
         112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,

         127,  126,  125,  124,  123,  122,  121,  120,  119,  118,  117,  116,  115,  114,  113,  112,
         111,  110,  109,  108,  107,  106,  105,  104,  103,  102,  101,  100,   99,   98,   97,   96,
          95,   94,   93,   92,   91,   90,   89,   88,   87,   86,   85,   84,   83,   82,   81,   80,
          79,   78,   77,   76,   75,   74,   73,   72,   71,   70,   69,   68,   67,   66,   65,   64,
          63,   62,   61,   60,   59,   58,   57,   56,   55,   54,   53,   52,   51,   50,   49,   48,
          47,   46,   45,   44,   43,   42,   41,   40,   39,   38,   37,   36,   35,   34,   33,   32,
          31,   30,   29,   28,   27,   26,   25,   24,   23,   22,   21,   20,   19,   18,   17,   16,
          15,   14,   13,   12,   11,   10,    9,    8,    7,    6,    5,    4,    3,    2,    1,    0,
          -1,   -2,   -3,   -4,   -5,   -6,   -7,   -8,   -9,  -10,  -11,  -12,  -13,  -14,  -15,  -16,
         -17,  -18,  -19,  -20,  -21,  -22,  -23,  -24,  -25,  -26,  -27,  -28,  -29,  -30,  -31,  -32,
         -33,  -34,  -35,  -36,  -37,  -38,  -39,  -40,  -41,  -42,  -43,  -44,  -45,  -46,  -47,  -48,
         -49,  -50,  -51,  -52,  -53,  -54,  -55,  -56,  -57,  -58,  -59,  -60,  -61,  -62,  -63,  -64,
         -65,  -66,  -67,  -68,  -69,  -70,  -71,  -72,  -73,  -74,  -75,  -76,  -77,  -78,  -79,  -80,
         -81,  -82,  -83,  -84,  -85,  -86,  -87,  -88,  -89,  -90,  -91,  -92,  -93,  -94,  -95,  -96,
         -97,  -98,  -99, -100, -101, -102, -103, -104, -105, -106, -107, -108, -109, -110, -111, -112,
        -113, -114, -115, -116, -117, -118, -119, -120, -121, -122, -123, -124, -125, -126, -127,

        -128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113,
        -112, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -100,  -99,  -98,  -97,
         -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,  -88,  -87,  -86,  -85,  -84,  -83,  -82,  -81,
         -80,  -79,  -78,  -77,  -76,  -75,  -74,  -73,  -72,  -71,  -70,  -69,  -68,  -67,  -66,  -65,
         -64,  -63,  -62,  -61,  -60,  -59,  -58,  -57,  -56,  -55,  -54,  -53,  -52,  -51,  -50,  -49,
         -48,  -47,  -46,  -45,  -44,  -43,  -42,  -41,  -40,  -39,  -38,  -37,  -36,  -35,  -34,  -33,
         -32,  -31,  -30,  -29,  -28,  -27,  -26,  -25,  -24,  -23,  -22,  -21,  -20,  -19,  -18,  -17,
         -16,  -15,  -14,  -13,  -12,  -11,  -10,   -9,   -8,   -7,   -6,   -5,   -4,   -3,   -2,   -1
    }
};

const WaveTable tiltedSawWave = WaveTable {
    .numSamples = 298,
    .samples = new signed char[298] {
           0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
          16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
          32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
          48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
          64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
          80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
          96,   97,   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
         112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,

         127,  121,  115,  109,  103,   97,   91,   85,   79,   73,   67,   61,   55,   49,   43,   37,
          31,   25,   19,   13,    7,    1,   -5,  -11,  -17,  -23,  -29,  -35,  -41,  -47,  -53,  -59,
         -65,  -71,  -77,  -83,  -89,  -95, -101, -107, -113, -119, -125,

        -128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113,
        -112, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -100,  -99,  -98,  -97,
         -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,  -88,  -87,  -86,  -85,  -84,  -83,  -82,  -81,
         -80,  -79,  -78,  -77,  -76,  -75,  -74,  -73,  -72,  -71,  -70,  -69,  -68,  -67,  -66,  -65,
         -64,  -63,  -62,  -61,  -60,  -59,  -58,  -57,  -56,  -55,  -54,  -53,  -52,  -51,  -50,  -49,
         -48,  -47,  -46,  -45,  -44,  -43,  -42,  -41,  -40,  -39,  -38,  -37,  -36,  -35,  -34,  -33,
         -32,  -31,  -30,  -29,  -28,  -27,  -26,  -25,  -24,  -23,  -22,  -21,  -20,  -19,  -18,  -17,
         -16,  -15,  -14,  -13,  -12,  -11,  -10,   -9,   -8,   -7,   -6,   -5,   -4,   -3,   -2,   -1
    }
};

const WaveTable sawWave = WaveTable {
    .numSamples = 256,
    .samples = new signed char[256] {
           0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
          16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
          32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
          48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
          64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
          80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
          96,   97,   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
         112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,  127,

        -128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113,
        -112, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -100,  -99,  -98,  -97,
         -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,  -88,  -87,  -86,  -85,  -84,  -83,  -82,  -81,
         -80,  -79,  -78,  -77,  -76,  -75,  -74,  -73,  -72,  -71,  -70,  -69,  -68,  -67,  -66,  -65,
         -64,  -63,  -62,  -61,  -60,  -59,  -58,  -57,  -56,  -55,  -54,  -53,  -52,  -51,  -50,  -49,
         -48,  -47,  -46,  -45,  -44,  -43,  -42,  -41,  -40,  -39,  -38,  -37,  -36,  -35,  -34,  -33,
         -32,  -31,  -30,  -29,  -28,  -27,  -26,  -25,  -24,  -23,  -22,  -21,  -20,  -19,  -18,  -17,
         -16,  -15,  -14,  -13,  -12,  -11,  -10,   -9,   -8,   -7,   -6,   -5,   -4,   -3,   -2,   -1
    }
};

const WaveTable squareWave = WaveTable {
    .numSamples = 2,
    .samples = new signed char[2] {
        127, -128
    }
};

const WaveTable pulseWave = WaveTable {
    .numSamples = 3,
    .samples = new signed char[3] {
        127, -128, -128,
    }
};

const WaveTable organWave = WaveTable {
    .numSamples = 1020,
    .samples = new signed char[1020] {
           0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
          16,   17,   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
          32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
          48,   49,   50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,
          64,   65,   66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,
          80,   81,   82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
          96,   97,   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
         112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,

         127,  126,  125,  124,  123,  122,  121,  120,  119,  118,  117,  116,  115,  114,  113,  112,
         111,  110,  109,  108,  107,  106,  105,  104,  103,  102,  101,  100,   99,   98,   97,   96,
          95,   94,   93,   92,   91,   90,   89,   88,   87,   86,   85,   84,   83,   82,   81,   80,
          79,   78,   77,   76,   75,   74,   73,   72,   71,   70,   69,   68,   67,   66,   65,   64,
          63,   62,   61,   60,   59,   58,   57,   56,   55,   54,   53,   52,   51,   50,   49,   48,
          47,   46,   45,   44,   43,   42,   41,   40,   39,   38,   37,   36,   35,   34,   33,   32,
          31,   30,   29,   28,   27,   26,   25,   24,   23,   22,   21,   20,   19,   18,   17,   16,
          15,   14,   13,   12,   11,   10,    9,    8,    7,    6,    5,    4,    3,    2,    1,    0,
          -1,   -2,   -3,   -4,   -5,   -6,   -7,   -8,   -9,  -10,  -11,  -12,  -13,  -14,  -15,  -16,
         -17,  -18,  -19,  -20,  -21,  -22,  -23,  -24,  -25,  -26,  -27,  -28,  -29,  -30,  -31,  -32,
         -33,  -34,  -35,  -36,  -37,  -38,  -39,  -40,  -41,  -42,  -43,  -44,  -45,  -46,  -47,  -48,
         -49,  -50,  -51,  -52,  -53,  -54,  -55,  -56,  -57,  -58,  -59,  -60,  -61,  -62,  -63,  -64,
         -65,  -66,  -67,  -68,  -69,  -70,  -71,  -72,  -73,  -74,  -75,  -76,  -77,  -78,  -79,  -80,
         -81,  -82,  -83,  -84,  -85,  -86,  -87,  -88,  -89,  -90,  -91,  -92,  -93,  -94,  -95,  -96,
         -97,  -98,  -99, -100, -101, -102, -103, -104, -105, -106, -107, -108, -109, -110, -111, -112,
        -113, -114, -115, -116, -117, -118, -119, -120, -121, -122, -123, -124, -125, -126, -127,

        -128, -127, -127, -126, -126, -125, -125, -124, -124, -123, -123, -122, -122, -121, -121, -120,
        -120, -119, -119, -118, -118, -117, -117, -116, -116, -115, -115, -114, -114, -113, -113, -112,
        -112, -111, -111, -110, -110, -109, -109, -108, -108, -107, -107, -106, -106, -105, -105, -104,
        -104, -103, -103, -102, -102, -101, -101, -100, -100,  -99,  -99,  -98,  -98,  -97,  -97,  -96,
         -96,  -95,  -95,  -94,  -94,  -93,  -93,  -92,  -92,  -91,  -91,  -90,  -90,  -89,  -89,  -88,
         -88,  -87,  -87,  -86,  -86,  -85,  -85,  -84,  -84,  -83,  -83,  -82,  -82,  -81,  -81,  -80,
         -80,  -79,  -79,  -78,  -78,  -77,  -77,  -76,  -76,  -75,  -75,  -74,  -74,  -73,  -73,  -72,
         -72,  -71,  -71,  -70,  -70,  -69,  -69,  -68,  -68,  -67,  -67,  -66,  -66,  -65,  -65,  -64,
         -64,  -63,  -63,  -62,  -62,  -61,  -61,  -60,  -60,  -59,  -59,  -58,  -58,  -57,  -57,  -56,
         -56,  -55,  -55,  -54,  -54,  -53,  -53,  -52,  -52,  -51,  -51,  -50,  -50,  -49,  -49,  -48,
         -48,  -47,  -47,  -46,  -46,  -45,  -45,  -44,  -44,  -43,  -43,  -42,  -42,  -41,  -41,  -40,
         -40,  -39,  -39,  -38,  -38,  -37,  -37,  -36,  -36,  -35,  -35,  -34,  -34,  -33,  -33,  -32,
         -32,  -31,  -31,  -30,  -30,  -29,  -29,  -28,  -28,  -27,  -27,  -26,  -26,  -25,  -25,  -24,
         -24,  -23,  -23,  -22,  -22,  -21,  -21,  -20,  -20,  -19,  -19,  -18,  -18,  -17,  -17,  -16,
         -16,  -15,  -15,  -14,  -14,  -13,  -13,  -12,  -12,  -11,  -11,  -10,  -10,   -9,   -9,   -8,
          -8,   -7,   -7,   -6,   -6,   -5,   -5,   -4,   -4,   -3,   -3,   -2,   -2,   -1,   -1,

           0,   -1,   -1,   -2,   -2,   -3,   -3,   -4,   -4,   -5,   -5,   -6,   -6,   -7,   -7,   -8,
          -8,   -9,   -9,  -10,  -10,  -11,  -11,  -12,  -12,  -13,  -13,  -14,  -14,  -15,  -15,  -16,
         -16,  -17,  -17,  -18,  -18,  -19,  -19,  -20,  -20,  -21,  -21,  -22,  -22,  -23,  -23,  -24,
         -24,  -25,  -25,  -26,  -26,  -27,  -27,  -28,  -28,  -29,  -29,  -30,  -30,  -31,  -31,  -32,
         -32,  -33,  -33,  -34,  -34,  -35,  -35,  -36,  -36,  -37,  -37,  -38,  -38,  -39,  -39,  -40,
         -40,  -41,  -41,  -42,  -42,  -43,  -43,  -44,  -44,  -45,  -45,  -46,  -46,  -47,  -47,  -48,
         -48,  -49,  -49,  -50,  -50,  -51,  -51,  -52,  -52,  -53,  -53,  -54,  -54,  -55,  -55,  -56,
         -56,  -57,  -57,  -58,  -58,  -59,  -59,  -60,  -60,  -61,  -61,  -62,  -62,  -63,  -63,  -64,
         -64,  -65,  -65,  -66,  -66,  -67,  -67,  -68,  -68,  -69,  -69,  -70,  -70,  -71,  -71,  -72,
         -72,  -73,  -73,  -74,  -74,  -75,  -75,  -76,  -76,  -77,  -77,  -78,  -78,  -79,  -79,  -80,
         -80,  -81,  -81,  -82,  -82,  -83,  -83,  -84,  -84,  -85,  -85,  -86,  -86,  -87,  -87,  -88,
         -88,  -89,  -89,  -90,  -90,  -91,  -91,  -92,  -92,  -93,  -93,  -94,  -94,  -95,  -95,  -96,
         -96,  -97,  -97,  -98,  -98,  -99,  -99, -100, -100, -101, -101, -102, -102, -103, -103, -104,
        -104, -105, -105, -106, -106, -107, -107, -108, -108, -109, -109, -110, -110, -111, -111, -112,
        -112, -113, -113, -114, -114, -115, -115, -116, -116, -117, -117, -118, -118, -119, -119, -120,
        -120, -121, -121, -122, -122, -123, -123, -124, -124, -125, -125, -126, -126, -127, -127,

        -128, -127, -126, -125, -124, -123, -122, -121, -120, -119, -118, -117, -116, -115, -114, -113,
        -112, -111, -110, -109, -108, -107, -106, -105, -104, -103, -102, -101, -100,  -99,  -98,  -97,
         -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,  -88,  -87,  -86,  -85,  -84,  -83,  -82,  -81,
         -80,  -79,  -78,  -77,  -76,  -75,  -74,  -73,  -72,  -71,  -70,  -69,  -68,  -67,  -66,  -65,
         -64,  -63,  -62,  -61,  -60,  -59,  -58,  -57,  -56,  -55,  -54,  -53,  -52,  -51,  -50,  -49,
         -48,  -47,  -46,  -45,  -44,  -43,  -42,  -41,  -40,  -39,  -38,  -37,  -36,  -35,  -34,  -33,
         -32,  -31,  -30,  -29,  -28,  -27,  -26,  -25,  -24,  -23,  -22,  -21,  -20,  -19,  -18,  -17,
         -16,  -15,  -14,  -13,  -12,  -11,  -10,   -9,   -8,   -7,   -6,   -5,   -4,   -3,   -2,   -1
    }
};

// Periods in samples for Octave = 0 and Sample rate = 44100
const int notePeriod[numNotes] = {
    802, 757, 714, 674, 636, 601, 567, 535, 505, 477, 450, 425
};

void TuneGenerator::setTuneSpec(const TuneSpec* tuneSpec) {
    _tuneSpec = tuneSpec;

    _note = _tuneSpec->notes;
    setSamplesPerNote();

    _waveTable = nullptr;
    startNote();
}

bool TuneGenerator::isFirstArpeggioNote() const {
    // Assumes we are in Arpeggio mode (i.e. _arpeggioNote is not null)
    int numNotes = 1 << arpeggioFactor(_arpeggioNote);
    const NoteSpec* firstArpNote =
        _arpeggioNote - (_arpeggioNote - _tuneSpec->notes) % numNotes;

    return _note == firstArpNote;
}

bool TuneGenerator::isLastArpeggioNote() const {
    // Assumes we are in Arpeggio mode (i.e. _arpeggioNote is not null)
    int numNotes = 1 << arpeggioFactor(_arpeggioNote);
    const NoteSpec* lastArpNote =
        _arpeggioNote - (_arpeggioNote - _tuneSpec->notes) % numNotes + numNotes - 1;

    return _note == lastArpNote;
}

const NoteSpec* TuneGenerator::peekPrevNote() const {
    if (_note == _tuneSpec->notes) {
        return nullptr;
    }

    if (_arpeggioNote != nullptr && isFirstArpeggioNote()) {
        if (_arpeggioNote == _tuneSpec->notes) {
            return nullptr;
        } else {
            return _arpeggioNote - 1;
        }
    } else {
        return _note - 1;
    }

}

const NoteSpec* TuneGenerator::peekNextNote() const {
    const NoteSpec* nextNote = _note + 1;
    const NoteSpec* lastNote = _tuneSpec->notes + _tuneSpec->loopEnd;

    if (_arpeggioNote != nullptr && isLastArpeggioNote()) {
        nextNote = _arpeggioNote + 1;
    }

    if (nextNote == lastNote) {
        const NoteSpec* resumeNote = _tuneSpec->notes + _tuneSpec->loopStart;
        if (resumeNote < lastNote) {
            nextNote = resumeNote;
        } else {
            nextNote = nullptr;
        }
    }

    return nextNote;
}

void TuneGenerator::moveToNextNote() {
    if (_arpeggioNote != nullptr && isLastArpeggioNote()) {
        // Exit Arpeggio mode
        _note = _arpeggioNote;
        _arpeggioNote = nullptr;
        setSamplesPerNote();
    }

    _note = peekNextNote();
}

void TuneGenerator::startNote() {
    if (
        (_note->fx == Effect::ARPEGGIO || _note->fx == Effect::ARPEGGIO_FAST) &&
        _arpeggioNote == nullptr
    ) {
        // Enter Arpeggio mode
        _arpeggioNote = _note;
        int factor = arpeggioFactor(_arpeggioNote);
        _note -= (_note - _tuneSpec->notes) % (1 << factor);
        _samplesPerNote >>= factor;
    }

    _sampleIndex = 0;

    const WaveTable* prevWaveTable = _waveTable;
    switch (_note->wav) {
        case WaveForm::TRIANGLE:
        case WaveForm::NOISE:
            _waveTable = &triangleWave; break;
        case WaveForm::TILTED_SAW:
            _waveTable = &tiltedSawWave; break;
        case WaveForm::SAW:
            _waveTable = &sawWave; break;
        case WaveForm::SQUARE:
            _waveTable = &squareWave; break;
        case WaveForm::PULSE:
            _waveTable = &pulseWave; break;
        case WaveForm::ORGAN:
            _waveTable = &organWave; break;
    }
    if (_waveTable != prevWaveTable) {
        _waveIndex = 0;
    }
    _maxWaveIndex = (_waveTable->numSamples << WAVETABLE_SHIFT);

    const NoteSpec* nxtNote = peekNextNote();
    if (nxtNote == nullptr || nxtNote->wav != _note->wav) {
        // Blend note to avoid transition artifacts
        _endMainIndex = _samplesPerNote - _note->vol;
    } else {
        _endMainIndex = _samplesPerNote;
    }

    int period = notePeriod[(int)_note->note] << (PERIOD_SHIFT - _note->oct);

    _indexDelta = (
        (_waveTable->numSamples << (WAVETABLE_SHIFT + PERIOD_SHIFT)) / period
    ) >> SAMPLERATE_SHIFT;
    _indexDeltaDelta = 0;

    _noiseShift = 0;
    if (_note->wav == WaveForm::NOISE) {
        _noiseShift = NUM_RAND_BITS - 2;
        int v = _indexDelta;
        while (v) {
            _noiseShift--;
            v >>= 1;
        }
    }

    _volume = (_note->vol << VOLUME_SHIFT) - 1;
    _volumeDelta = 0;

    switch (_note->fx) {
        case Effect::FADE_IN:
            _volumeDelta = _volume / _samplesPerNote;
            _volume = 0;
            break;
        case Effect::FADE_OUT:
            _volumeDelta = -(_volume / _samplesPerNote);
            break;
        case Effect::SLIDE:
            if (nxtNote != nullptr) {
                int nxtVol = (nxtNote->vol << VOLUME_SHIFT) - 1;
                _volumeDelta = (nxtVol - _volume) / _samplesPerNote;

                int nxtPeriod = notePeriod[(int)nxtNote->note] << (PERIOD_SHIFT - nxtNote->oct);
                int nxtIndexDelta = (
                    (_waveTable->numSamples << (WAVETABLE_SHIFT + PERIOD_SHIFT)) / nxtPeriod
                ) >> SAMPLERATE_SHIFT;
                _indexDeltaDelta = (nxtIndexDelta - _indexDelta) / _samplesPerNote;
            }
            break;
        case Effect::ARPEGGIO:
        case Effect::ARPEGGIO_FAST:
            // Ignore (as we are already playing note at Arpeggio speed when we reach this point)
            break;

        case Effect::DROP: {
                int nxtIndexDelta = _indexDelta >> 1;
                _indexDeltaDelta = (nxtIndexDelta - _indexDelta) / _samplesPerNote;
            }
            break;
        case Effect::VIBRATO: {
            const NoteSpec* prevNote = peekPrevNote();
            if (prevNote == nullptr || prevNote->fx != Effect::VIBRATO) {
                _vibratoDelta = 0;
                // Note: The order of operations matters to avoid overflows during calculation
                _vibratoDeltaDelta = _indexDelta / (
                    VIBRATO_META_PERIOD * (_maxWaveIndex / _indexDelta)
                );
            }
        }
        case Effect::NONE:
            break;
    }
}

void TuneGenerator::addMainSamples(Sample* &curP, Sample* endP) {
    _sampleIndex += endP - curP; // Update beforehand
    while (curP < endP) {
        int sample = _waveTable->samples[_waveIndex >> WAVETABLE_SHIFT];
        _waveIndex += _indexDelta;
        if (_noiseShift) {
            int rnd = rand();
            if (rnd & 1) {
                _waveIndex += rnd >> _noiseShift;
            } else {
                _waveIndex -= rnd >> _noiseShift;
                if (_waveIndex < 0) {
                    _waveIndex += _maxWaveIndex;
                }
            }
        }
        if (_waveIndex >= _maxWaveIndex) {
            _waveIndex -= _maxWaveIndex;
        }
        _indexDelta += _indexDeltaDelta;

        sample *= _volume;
        _volume += _volumeDelta;
        *curP++ = (char)(128 + (sample >> (VOLUME_SHIFT + VOLUME_BITS)));
    }
}

// Separate method for Vibrato effect for efficiency. This goes both ways. The calculations here
// are Vibrato-specific, and when Vibrato is applied, there are no changes of volume or other
// frequency shifts.
void TuneGenerator::addMainSamplesVibrato(Sample* &curP, Sample* endP) {
    _sampleIndex += endP - curP; // Update beforehand
    while (curP < endP) {
        int sample = _waveTable->samples[_waveIndex >> WAVETABLE_SHIFT];
        _waveIndex += _indexDelta;
        _waveIndex += _vibratoDelta >> VIBRATO_MAGNITUDE_SHIFT;
        if (_waveIndex >= _maxWaveIndex) {
            _waveIndex -= _maxWaveIndex;
        } else if (_waveIndex < 0) {
            _waveIndex += _maxWaveIndex;
        }

        _vibratoDelta += _vibratoDeltaDelta;
        if (abs(_vibratoDelta) > _indexDelta) {
            _vibratoDeltaDelta = -_vibratoDeltaDelta;
        }

        sample *= _volume;
        *curP++ = (char)(128 + (sample >> (VOLUME_SHIFT + VOLUME_BITS)));
    }
}

void TuneGenerator::addBlendSamples(Sample* &curP, Sample* endP) {
    if (_sampleIndex == _endMainIndex) {
        int finalSample = 0;
        const NoteSpec* nxtNote = peekNextNote();
        if (
            nxtNote != nullptr && (
                nxtNote->wav == WaveForm::SQUARE || nxtNote->wav == WaveForm::PULSE
            )
        ) {
            // Exception. These waves do not start at zero.
            finalSample = 127 << (VOLUME_SHIFT + VOLUME_BITS);
        }
        _blendDelta = (finalSample - _blendSample) / (_samplesPerNote - _endMainIndex + 1);
    }

    _sampleIndex += endP - curP; // Update beforehand
    while (curP < endP) {
        _blendSample += _blendDelta;

        *curP++ = (char)(128 + (_blendSample >> (VOLUME_SHIFT + VOLUME_BITS)));
    }
}

int TuneGenerator::addSamples(Sample* buf, int maxSamples) {
    Sample* bufP = buf;
    Sample* maxBufP = bufP + maxSamples;

    while (bufP < maxBufP) {
        if (_sampleIndex < _endMainIndex) {
            // Add main samples until end of main phase or buffer is full
            int numSamples = std::min(_endMainIndex - _sampleIndex, (int)(maxBufP - bufP));
            if (_note->fx == Effect::VIBRATO) {
                addMainSamplesVibrato(bufP, bufP + numSamples);
            } else {
                addMainSamples(bufP, bufP + numSamples);
            }
            if (_sampleIndex == _endMainIndex) {
                _blendSample = (*(bufP - 1) - 128) << (VOLUME_SHIFT + VOLUME_BITS);
            }
        } else {
            // Add ramp-down samples until end of note or buffer is full
            int numSamples = std::min(_samplesPerNote - _sampleIndex, (int)(maxBufP - bufP));
            addBlendSamples(bufP, bufP + numSamples);
        }

        if (_sampleIndex == _samplesPerNote) {
            if (_note == nullptr) {
                // End of tune
                break;
            }
            moveToNextNote();
            if (_note == nullptr) {
                // End of tune
                break;
            }

            startNote();
        }
    }

    return (int)(bufP - buf);
}
