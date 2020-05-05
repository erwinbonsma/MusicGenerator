//
//  Music.cpp
//  MusicGB
//
//  Created by Erwin on 04/04/2020.
//  Copyright © 2020 Erwin. All rights reserved.
//

#include "Music.h"

#ifdef STANDALONE
  #include <cstdlib>
  #include <stdio.h>
  #include <assert.h>
  #define min(x,y) ((x)<(y)?(x):(y))
#else
  #define assert(condition)
  namespace Gamebuino_Meta {
#endif
#ifdef UNDEFINED
  }
#endif

// Periods in samples for Octave = 0 and Sample rate = 11025, and shifted SPEC_PERIOD_SHIFT bits
// (for maximum accuracy).
const int16_t notePeriodLookup[numNotes] = {
    21576, 20365, 19222, 18143, 17125, 16164, 15256, 14400, 13592, 12829, 12109, 11429
};
constexpr uint8_t SPEC_PERIOD_SHIFT = 5;

constexpr uint8_t MAX_OCTAVE = 7;
constexpr uint8_t PERIOD_SHIFT = MAX_OCTAVE + SPEC_PERIOD_SHIFT;

// Controls the amount of frequency change.
constexpr uint8_t VIBRATO_MAGNITUDE_SHIFT = 7;

// The amount of wave periods spanned by a single period of the Vibrato effect.
constexpr uint8_t VIBRATO_META_PERIOD = 24;

// The volume from the note spec uses up to four bits, as the maximum value is eight. Note, this
// maximum should be a power of two to ensure that at maximum volume each output value in the valid
// range (i.e. [-128..127]) can actually be realized (so that the available resolution is fully
// utilized).
constexpr uint8_t VOLUME_BITS = 4;

// Shift such that shifted volume fits in four bytes without overwriting the sign bit. This way the
// most-significant byte can be safely cast to (signed) int8_t value.
constexpr uint8_t VOLUME_SHIFT = 32 - VOLUME_BITS - 1;

// Number of bits to shift amplified sample so that it is in range [-128..127] again.
///  Inputs: - sample with range [-128..127]
//           - volume with range [   0.. 64] (the range of the most-significant byte)
constexpr uint8_t POST_AMP_SHIFT = 6;

// Notes:
// - The size of wave tables varies to match the wave form to avoid rounding-artifacts and to
//   efficiently capture each. This is compensated for during sample generation.
// - Where possible, wave forms start at 0x80 (the zero position) so that there's no need to ramp
//   up after a change of instrument.
const int8_t triangleWaveSamples[510] = {
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
};
const WaveTable triangleWave = WaveTable {
    .numSamples = 510,
    .initialWaveIndex = 0,
    .shift = 6 + 15,
    .samples = triangleWaveSamples
};

const int8_t tiltedSawWaveSamples[298] = {
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
};
const WaveTable tiltedSawWave = WaveTable {
    .numSamples = 298,
    .initialWaveIndex = 0,
    .shift = 6 + 15,
    .samples = tiltedSawWaveSamples
};

const int8_t sawWaveSamples[256] = {
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
};
const WaveTable sawWave = WaveTable {
    .numSamples = 256,
    .initialWaveIndex = 0,
    .shift = 6 + 15,
    .samples = sawWaveSamples
};

const int8_t squareWaveSamples[2] = { 127, -128 };
const WaveTable squareWave = WaveTable {
    .numSamples = 2,
    .initialWaveIndex = 0,
    .shift = 13 + 15,
    .samples = squareWaveSamples
};

const int8_t pulseWaveSamples[3] = { 127, -128, -128 };
const WaveTable pulseWave = WaveTable {
    .numSamples = 3,
    .initialWaveIndex = 0,
    .shift = 13 + 15,
    .samples = pulseWaveSamples
};

const int8_t organWaveSamples[1020] = {
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
};
const WaveTable organWave = WaveTable {
    .numSamples = 1020,
    .initialWaveIndex = 0,
    .shift = 5 + 15,
    .samples = organWaveSamples
};

// Triangle wave with Pelgrin noise added on top.
const int8_t noiseWaveSamples[510] = {
       4,    8,   11,   13,   16,   18,   19,   20,   22,   24,   27,   29,   31,   33,   37,   38,
      40,   40,   41,   42,   43,   43,   43,   44,   45,   44,   43,   43,   43,   42,   43,   42,
      41,   44,   47,   49,   52,   54,   54,   57,   59,   62,   64,   67,   68,   73,   76,   80,
      85,   84,   84,   83,   83,   83,   83,   83,   84,   84,   85,   87,   89,   89,   89,   89,
      90,   90,   91,   90,   90,   90,   91,   91,   90,   90,   90,   92,   94,   93,   94,   94,
      95,   93,   90,   90,   89,   87,   86,   83,   81,   81,   83,   81,   80,   80,   79,   80,
      81,   82,   82,   83,   85,   85,   85,   85,   86,   84,   83,   82,   82,   81,   80,   80,
      79,   79,   79,   82,   83,   84,   84,   85,   85,   85,   85,   85,   85,   87,   88,   88,
      88,   90,   90,   93,   96,   95,   95,   95,   97,   98,   98,   98,   98,   98,   98,   99,
     100,  101,  103,  106,  108,  108,  108,  110,  110,  112,  114,  116,  118,  119,  120,  121,
     123,  122,  122,  121,  120,  119,  120,  118,  117,  119,  122,  122,  123,  125,  127,
     126,  123,  121,  118,  115,  116,  115,  115,  115,  110,  106,  101,   97,   94,   91,   88,
      86,   85,   83,   80,   77,   76,   73,   71,   68,   67,   65,   62,   60,   57,   54,   51,
      47,   49,   49,   50,   51,   50,   49,   48,   48,   46,   44,   40,   37,   36,   35,   32,
      29,   29,   29,   30,   31,   29,   28,   28,   26,   24,   23,   23,   23,   21,   19,   18,
      16,   16,   16,   14,   12,   11,    9,    9,    7,    7,    7,    6,    7,    5,    4,    2,
       0,   -1,   -3,   -3,   -4,   -6,   -6,   -9,  -12,  -10,   -9,  -11,  -12,  -11,  -12,  -10,
      -9,  -11,  -13,  -17,  -20,  -20,  -21,  -23,  -24,  -28,  -32,  -34,  -37,  -38,  -38,  -40,
     -41,  -44,  -48,  -49,  -51,  -55,  -58,  -63,  -66,  -69,  -72,  -76,  -79,  -81,  -84,  -87,
     -90,  -88,  -85,  -83,  -82,  -83,  -82,  -84,  -84,  -82,  -80,  -79,  -78,  -77,  -76,  -75,
     -73,  -72,  -71,  -70,  -69,  -70,  -71,  -70,  -70,  -69,  -67,  -65,  -63,  -63,  -63,  -61,
     -59,  -61,  -63,  -63,  -64,  -64,  -64,  -63,  -63,  -65,  -67,  -67,  -67,  -67,  -69,  -67,
     -67,  -68,  -69,  -70,  -71,  -72,  -74,  -73,  -73,  -76,  -79,  -80,  -82,  -83,  -85,  -86,
     -87,  -88,  -88,  -89,  -89,  -90,  -89,  -89,  -89,  -92,  -94,  -96,  -98,  -98,  -99, -100,
    -101, -102, -103, -104, -106, -109, -111, -113, -116, -117, -119, -120, -123, -123, -123, -125,
    -128, -126, -123, -122, -121, -119, -116, -114, -112, -113, -114, -115, -116, -116, -116, -115,
    -113, -114, -114, -115, -115, -116, -117, -116, -115, -117, -118, -121, -123, -123, -124,
    -125, -122, -120, -116, -113, -109, -105, -101,  -98,  -97,  -96,  -93,  -91,  -88,  -86,  -83,
     -79,  -78,  -76,  -73,  -71,  -69,  -67,  -63,  -60,  -57,  -55,  -52,  -49,  -47,  -45,  -43,
     -39,  -39,  -39,  -39,  -40,  -38,  -37,  -36,  -35,  -33,  -32,  -31,  -30,  -28,  -25,  -24,
     -22,  -21,  -20,  -19,  -17,  -14,  -13,  -10,   -7,   -5,   -2,   -1,    0,    3,    6,    9,
      12,   11,   11,   11,   11,   11,   12,   12,   13,   11,   10,    8,    7,    7,    6,    6
};
constexpr int numNoiseSamples = 510;
constexpr int noiseShift = 6 + 15;
const WaveTable noiseWave = WaveTable {
    .numSamples = numNoiseSamples,
    // Start at second-zerro crossing so that end of (first) wave is reached earlier, as only then
    // is frequency adapted every quarter-period.
    .initialWaveIndex = 255,
    .shift = noiseShift,
    .samples = noiseWaveSamples
};
constexpr int phaserShift = 21;
const WaveTable phaserWave = WaveTable {
    .numSamples = 512,
    .initialWaveIndex = 128,
    .shift = phaserShift,
    .samples = nullptr // Not used. Samples are calculated for this instrument
};

const WaveTable* waveTableLookup[9] = {
    &triangleWave,
    &tiltedSawWave,
    &sawWave,
    &squareWave,
    &pulseWave,
    &organWave,
    &noiseWave,
    &phaserWave,
    nullptr
};

const NoteSpec silentNote = NoteSpec {
    .note = Note::C4, .vol = 0, .wav = WaveForm::NONE, .fx = Effect::NONE
};

#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

inline void clearBuffer(int16_t* buf, int num) {
    // Note: implementation assumes that num > 0
    int16_t* endP = buf + num;
    do {
        *buf = 0;
    } while (++buf != endP);
}

inline int notePeriod(const NoteSpec* note) {
    return ((int)notePeriodLookup[(int)note->note & 0x0f]) << (MAX_OCTAVE - ((int)note->note >> 4));
}

int TuneSpec::lengthInTicks() const {
    return numNotes * noteDuration;
}

void TuneGenerator::setTuneSpec(const TuneSpec* tuneSpec, bool isFirst) {
    _tuneSpec = tuneSpec;

    setSamplesPerNote();

    if (_tuneSpec->notes == nullptr) {
        // The tune is a silent tune. It can be used to control the length of a pattern, when all
        // the other tunes are looping.
        _noteIndex = 0;
        _note = &silentNote; // Make it non-null, as it's used to signal tune termination
    } else {
        _note = _tuneSpec->notes;
    }

    _arpeggioNote = nullptr;
    _waveTable = nullptr;
    startNote();
    if (isFirst) {
        _numBlendSamples = 0;
    } else {
        createIncomingBlendSamplesIfNeeded();
    }
}

const NoteSpec* TuneGenerator::peekPrevNote() const {
    assert(_arpeggioNote == nullptr);

    if (_tuneSpec->notes == nullptr) {
        return (_noteIndex > 0) ? _note :  nullptr;
    }

    return (_note == _tuneSpec->notes) ? nullptr : _note - 1;
}

const NoteSpec* TuneGenerator::peekNextNote() const {
    assert(_arpeggioNote == nullptr);

    if (_tuneSpec->notes == nullptr) {
        return ((_noteIndex + 1) < _tuneSpec->numNotes) ? _note : nullptr;
    }

    const NoteSpec* nextNote = _note + 1;
    const NoteSpec* lastNote = _tuneSpec->notes + _tuneSpec->numNotes;

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
    if (_arpeggioNote != nullptr) {
        if (_pendingArpeggioSamples > 0) {
            if (_note == lastArpeggioNote()) {
                _note = firstArpeggioNote();
                return;
            } else {
                _note++;
                return;
            }
        } else {
            exitArpeggio();
        }
    }

    _note = peekNextNote();

    if (_tuneSpec->notes == nullptr) {
        ++_noteIndex;
    }
}

bool TuneGenerator::setWaveTable(WaveForm waveForm) {
    const WaveTable* prevWaveTable = _waveTable;
    _waveTable = waveTableLookup[(int)waveForm];

    if (_waveTable == nullptr) {
        _sampleGeneratorFun = &TuneGenerator::addMainSamplesSilence;
        return false; // Can (and should) skip remainder.
    }

    if (
        _waveTable != prevWaveTable ||
        _sampleGeneratorFun == &TuneGenerator::addMainSamplesVibrato
    ) {
        switch (waveForm) {
            case WaveForm::NOISE:
                _sampleGeneratorFun = &TuneGenerator::addMainSamplesNoise;
                break;
            case WaveForm::PHASER:
                _sampleGeneratorFun = &TuneGenerator::addMainSamplesPhaser;
                break;
            default:
                _sampleGeneratorFun = &TuneGenerator::addMainSamples;
                break;
        }

        if (_waveTable != prevWaveTable) {
            // Update the maximum for the new table
            _maxWaveIndex = _waveTable->numSamples << _waveTable->shift;

            // Change of instrument. Let it start at zero point
            _waveIndex = _waveTable->initialWaveIndex << _waveTable->shift;

            // Also reset instrument specific fields
            _indexNoiseDelta = 0;
            _phaserCount = 0;
        }
    }

    return true;
}

void TuneGenerator::startArpeggio() {
    // Enter Arpeggio mode
    _arpeggioNote = _note;

    _note -= (_note - _tuneSpec->notes) % 4;
    _pendingArpeggioSamples = _samplesPerNote;

    int noteDuration = _arpeggioNote->fx == Effect::ARPEGGIO ? 8 : 4;
    if (_tuneSpec->noteDuration <= 8) {
        noteDuration >>= 1;
    }
    _samplesPerNote = (noteDuration * SAMPLES_PER_TICK) << SAMPLERATE_SHIFT;

    setWaveTable(_arpeggioNote->wav);

    _volume = _arpeggioNote->vol << VOLUME_SHIFT;
    _volumeDelta = 0;
}

void TuneGenerator::exitArpeggio() {
    assert(_arpeggioNote != nullptr);
    assert(_pendingArpeggioSamples == 0);

    // Exit Arpeggio mode
    _note = _arpeggioNote;
    _arpeggioNote = nullptr;
    setSamplesPerNote();
}

int TuneGenerator::ticksPlayed() {
    if (_tuneSpec == nullptr) {
        return 0;
    }
    if (_note == nullptr) {
        return _tuneSpec->lengthInTicks();
    }

    int notesPlayed;
    if (_tuneSpec->notes == nullptr) {
        notesPlayed = _noteIndex;
    } else {
        notesPlayed = (int)((_arpeggioNote != nullptr ? _arpeggioNote : _note) - _tuneSpec->notes);
    }

    int ticksPlayedInCurrentNote = (_sampleIndex >> SAMPLERATE_SHIFT) / SAMPLES_PER_TICK;

    return notesPlayed * _tuneSpec->noteDuration + ticksPlayedInCurrentNote;
}

void TuneGenerator::startNote() {
    _sampleIndex = 0;

    if (_arpeggioNote == nullptr) {
        if (_note->fx == Effect::ARPEGGIO || _note->fx == Effect::ARPEGGIO_FAST) {
            startArpeggio();
        } else {
            // Set wave
            if (!setWaveTable(_note->wav)) {
                return;
            }

            // Set volume
            _volume = _note->vol << VOLUME_SHIFT;
            _volumeDelta = 0;
        }
    }

    // Ensure note plays at desired frequency
    _indexDelta = (
        (_waveTable->numSamples << _waveTable->shift) / notePeriod(_note)
    ) << (PERIOD_SHIFT - SAMPLERATE_SHIFT);
    _indexDeltaDelta = 0;

    if (_arpeggioNote != nullptr) {
        _samplesPerNote = min(_samplesPerNote, _pendingArpeggioSamples);
        _pendingArpeggioSamples -= _samplesPerNote; // Update beforehand

        // Nothing more needs doing. Effects don't apply.
        return;
    }

    switch (_note->fx) {
        case Effect::FADE_IN:
            _volumeDelta = _volume / _samplesPerNote;
            _volume = 0;
            break;
        case Effect::FADE_OUT:
            _volumeDelta = -(_volume / _samplesPerNote);
            break;
        case Effect::SLIDE: {
            const NoteSpec* prvNote = peekPrevNote();

            if (prvNote != nullptr) {
                // Slide from previous volume to own
                int prvVolume = (prvNote->vol << VOLUME_SHIFT);
                _volumeDelta = (_volume - prvVolume) / _samplesPerNote;
                _volume = prvVolume;

                // Slide from previous frequency to own
                int prvIndexDelta = (
                    (_waveTable->numSamples << _waveTable->shift) / notePeriod(prvNote)
                ) << (PERIOD_SHIFT - SAMPLERATE_SHIFT);
                _indexDeltaDelta = (_indexDelta - prvIndexDelta) / _samplesPerNote;
                _indexDelta = prvIndexDelta;
            }

            break;
        }
        case Effect::ARPEGGIO:
        case Effect::ARPEGGIO_FAST:
            assert(false);
            break;

        case Effect::DROP: {
            int nxtIndexDelta = _indexDelta >> 1;
            _indexDeltaDelta = (nxtIndexDelta - _indexDelta) / _samplesPerNote;

            break;
        }
        case Effect::VIBRATO:
            if (_sampleGeneratorFun == &TuneGenerator::addMainSamples) {
                // Currently only support Vibrato for simple instruments (not NOISE and PHASER)
                _sampleGeneratorFun = &TuneGenerator::addMainSamplesVibrato;

                const NoteSpec* prevNote = peekPrevNote();
                if (prevNote == nullptr || prevNote->fx != Effect::VIBRATO) {
                    _vibratoDelta = 0;
                    // Note: The order of operations matters to avoid overflows during calculation
                    _vibratoDeltaDelta = _indexDelta / (
                        VIBRATO_META_PERIOD * (_maxWaveIndex / _indexDelta)
                    );
                }
            }
            break;
        case Effect::NONE:
            break;
    }
}

void TuneGenerator::addMainSamples(Sample* &curP, Sample* endP) {
    int shift = _waveTable->shift;
    const int8_t* samples = _waveTable->samples;

    _sampleIndex += endP - curP; // Update beforehand
    while (curP < endP) {
        int8_t sample = samples[_waveIndex >> shift];
        _waveIndex += _indexDelta;
        if (_waveIndex >= _maxWaveIndex) {
            _waveIndex -= _maxWaveIndex;
        }
        _indexDelta += _indexDeltaDelta;

        int16_t amplifiedSample = sample * (int8_t)(_volume >> 24);
        _volume += _volumeDelta;
        *curP++ += amplifiedSample >> POST_AMP_SHIFT;
    }
}

void TuneGenerator::addMainSamplesPhaser(Sample* &curP, Sample* endP) {
    int p = ((_phaserCount < 128) ? _phaserCount : 255 - _phaserCount) << 9;
    int m = p + (((0x1 << 16) - p) >> 1);
    int hm = (m + 1) >> 1;

    _sampleIndex += endP - curP; // Update beforehand
    while (curP < endP) {
        int t = (_waveIndex >> (phaserShift - 8)) & 0xffff; // Throw away most significant bit
        int s = (t < p) ? t : p + ((t - p) >> 1);
        if (_waveIndex & (0x1 << (phaserShift + 8))) { // Check most significant bit
            s = (hm - s) >> 8;
        } else {
            s = (s - hm) >> 8;
        }

        _waveIndex += _indexDelta;
        if (_waveIndex >= _maxWaveIndex) {
            _waveIndex -= _maxWaveIndex;
            _phaserCount += 2;
            p = ((_phaserCount < 128) ? _phaserCount : 255 - _phaserCount) << 9;
            m =  p + (((0x1 << 16) - p) >> 1);
            hm = (m + 1) >> 1;
        }
        _indexDelta += _indexDeltaDelta;

        int16_t amplifiedSample = s * (int8_t)(_volume >> 24);
        _volume += _volumeDelta;
        *curP++ += amplifiedSample >> POST_AMP_SHIFT;
    }
}

void TuneGenerator::addMainSamplesNoise(Sample* &curP, Sample* endP) {
    constexpr int maxWaveIndexFullRange = numNoiseSamples << noiseShift;
    const int8_t* samples = _waveTable->samples;

    _sampleIndex += endP - curP; // Update beforehand
    while (curP < endP) {
        int8_t sample = samples[_waveIndex >> noiseShift];
        _waveIndex += _indexDelta;
        _waveIndex += _indexNoiseDelta;
        if (_waveIndex >= _maxWaveIndex) {
           // Determine new frequency multiplier
           _indexNoiseDelta = 0;
            while (1) {
                bool bit = (_noiseLfsr ^ (_noiseLfsr >> 1)) & 1;
                _noiseLfsr = (_noiseLfsr >> 1) ^ (bit << 14);
                if (bit) {
                    break;
                }
                _indexNoiseDelta += _indexDelta;
            };
            // Update max wave index to end of the waveform quadrant we just entered
            if (_maxWaveIndex == maxWaveIndexFullRange) {
                _waveIndex -= maxWaveIndexFullRange;
                _maxWaveIndex = 0;
            } else {
                _maxWaveIndex += (maxWaveIndexFullRange >> 2);
            }
        }
        _indexDelta += _indexDeltaDelta;

        int16_t amplifiedSample = sample * (int8_t)(_volume >> 24);
        _volume += _volumeDelta;
        *curP++ += amplifiedSample >> POST_AMP_SHIFT;
    }
}

void TuneGenerator::addMainSamplesSilence(Sample* &curP, Sample* endP) {
    _sampleIndex += endP - curP; // Update beforehand
    curP = endP;
}

// Separate method for Vibrato effect for efficiency. This goes both ways. The calculations here
// are Vibrato-specific, and when Vibrato is applied, there are no changes of volume or other
// frequency shifts.
void TuneGenerator::addMainSamplesVibrato(Sample* &curP, Sample* endP) {
    int shift = _waveTable->shift;
    const int8_t* samples = _waveTable->samples;

    _sampleIndex += endP - curP; // Update beforehand
    while (curP < endP) {
        int8_t sample = samples[_waveIndex >> shift];
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

        int16_t amplifiedSample = sample * (int8_t)(_volume >> 24);
        *curP++ += amplifiedSample >> POST_AMP_SHIFT;
    }
}

// Provide a smooth transition between subsequent notes
void TuneGenerator::addBlendSamples(Sample* &curP, Sample* endP) {
    int idx = NUM_BLEND_SAMPLES - _numBlendSamples;

    while (curP < endP) {
        int16_t _blendSample = (
            _numBlendSamples * _blendBufOut[idx] +
            (NUM_BLEND_SAMPLES - _numBlendSamples) * _blendBufIn[idx]
        ) >> BLEND_SHIFT;

        *curP++ += _blendSample;
        ++idx;
        --_numBlendSamples;
    }
}

void TuneGenerator::createOutgoingBlendSamplesIfNeeded() {
    const NoteSpec* nxt;
    const NoteSpec* cur;

    if (_arpeggioNote != nullptr) {
        if (_pendingArpeggioSamples) {
            // No blending required during Arpeggio transitions. All notes are the same volume and
            // wave, without effects.
            return;
        }

        nxt = _arpeggioNote + 1;
        if (nxt == _tuneSpec->notes + _tuneSpec->numNotes) {
            nxt = nullptr;
        }
        cur = _arpeggioNote;
    } else {
        nxt = peekNextNote();
        cur = _note;
    }

    if (nxt != nullptr && nxt->wav == cur->wav) {
        if (
            nxt->vol == cur->vol &&
            (nxt->fx == Effect::NONE || nxt->fx == Effect::SLIDE || nxt->fx == Effect::FADE_OUT) &&
            (cur->fx == Effect::NONE || cur->fx == Effect::SLIDE || cur->fx == Effect::FADE_IN)
        ) {
            // No blend required. Transition is smooth by itself
            return;
        }
        if (
            nxt->fx == Effect::SLIDE &&
            (cur->fx == Effect::NONE || cur->fx == Effect::SLIDE || cur->fx == Effect::FADE_IN)
        ) {
            return;
        }
    }

    Sample* buf = _blendBufOut;
    clearBuffer(buf, NUM_BLEND_SAMPLES);
    int32_t origWaveIndex = _waveIndex;
    CALL_MEMBER_FN(*this, _sampleGeneratorFun)(buf, buf + NUM_BLEND_SAMPLES);
    _numBlendSamples = NUM_BLEND_SAMPLES;
    _waveIndex = origWaveIndex; // Restore (this avoids artifacts when wave remains unchanged)
}

void TuneGenerator::createIncomingBlendSamplesIfNeeded() {
    if (_numBlendSamples == 0) {
        return;
    }

    // There are blend samples from the previous note. Fill own buffer so we need to start with
    // blending.

    Sample* buf = _blendBufIn;
    clearBuffer(buf, NUM_BLEND_SAMPLES);
    CALL_MEMBER_FN(*this, _sampleGeneratorFun)(buf, buf + NUM_BLEND_SAMPLES);
}

// The output level is double the volume for normal notes (for maximum accuracy)
int TuneGenerator::outputLevel() {
    if (_note == nullptr) {
        return 0;
    }
    if (_note->fx == Effect::SLIDE && _note != _tuneSpec->notes) {
        // Average over both volumes
        return (_note - 1)->vol + _note->vol;
    }
    if (_note->fx == Effect::FADE_IN || _note->fx == Effect::FADE_OUT) {
        // Half the volume
        return _note->vol;
    }
    return _note->vol << 1;
}

int TuneGenerator::addSamples(Sample* buf, int maxSamples) {
    if (_note == nullptr) {
        // End of tune
        return 0;
    }

    Sample* bufP = buf;
    Sample* maxBufP = bufP + maxSamples;

    while (bufP < maxBufP) {
        if (_numBlendSamples > 0) {
            // Add blend samples until they are exhausted or buffer is full
            int numSamples = min(_numBlendSamples, (int)(maxBufP - bufP));
            addBlendSamples(bufP, bufP + numSamples);
        } else if (_sampleIndex < _samplesPerNote) {
            // Add main samples until end of main phase or buffer is full
            int numSamples = min(_samplesPerNote - _sampleIndex, (int)(maxBufP - bufP));
            CALL_MEMBER_FN(*this, _sampleGeneratorFun)(bufP, bufP + numSamples);
        }

        if (_sampleIndex >= _samplesPerNote) {
            createOutgoingBlendSamplesIfNeeded();
            moveToNextNote();

            if (_note == nullptr) {
                // End of tune
                break;
            }

            startNote();
            createIncomingBlendSamplesIfNeeded();
        }
    }

    return (int)(bufP - buf);
}

//--------------------------------------------------------------------------------------------------
// Pattern Generation

int PatternSpec::lengthInTicks() const {
    return tunes[0]->lengthInTicks();
}

void PatternGenerator::setPatternSpec(const PatternSpec* patternSpec, bool isFirst) {
    _patternSpec = patternSpec;
    for (int i = 0; i < _patternSpec->numTunes; i++) {
        _tuneGens[i].setTuneSpec(_patternSpec->tunes[i], isFirst);
    }
}

int PatternGenerator::ticksPlayed() {
    return _tuneGens[0].ticksPlayed();
}

int PatternGenerator::outputLevel() {
    int sum = 0;

    for (int i = _patternSpec->numTunes; --i >= 0; ) {
        sum += _tuneGens[i].outputLevel();
    }

    return sum;
}

int PatternGenerator::addSamples(Sample* buf, int maxSamples) {
    // The first tune detemines the length of the pattern. It should therefore not loop.
    int numSamples = _tuneGens[0].addSamples(buf, maxSamples);

    for (int i = 1; i < _patternSpec->numTunes; i++) {
        _tuneGens[i].addSamples(buf, numSamples);
    }

    return numSamples;
}

//--------------------------------------------------------------------------------------------------
// Song Generation

int SongSpec::lengthInTicks() const {
    int len = 0;
    for (int i = numPatterns; --i >= 0; ) {
        len += patterns[i]->lengthInTicks();
    }

    return len;
}

int SongSpec::lengthInSeconds() const {
    // Adding "SAMPLERATE - 1" to round up.
    return (lengthInTicks() * SAMPLES_PER_TICK + SAMPLERATE - 1) / SAMPLERATE;
}

void SongGenerator::startPattern(bool isFirst) {
    _patternGenerator.setPatternSpec(*_pattern, isFirst);
}

void SongGenerator::moveToNextPattern() {
    _ticksPlayedInEarlierPatterns += (*_pattern)->lengthInTicks();

    _pattern++;

    if (_pattern == _songSpec->patterns + _songSpec->numPatterns) {
        if (_loop && _songSpec->loopStart < _songSpec->numPatterns) {
            const PatternSpec *const * loopStart = _songSpec->patterns + _songSpec->loopStart;
            _pattern = _songSpec->patterns;
            _ticksPlayedInEarlierPatterns = 0;

            while (_pattern != loopStart) {
                _ticksPlayedInEarlierPatterns += (*_pattern)->lengthInTicks();
                ++_pattern;
            }
        } else {
            _pattern = nullptr;
        }
    }
}

void SongGenerator::setSongSpec(const SongSpec* songSpec, bool loop) {
    _songSpec = songSpec;
    _pattern = _songSpec->patterns;
    _loop = loop;
    _ticksPlayedInEarlierPatterns = 0;
    startPattern(true);
}


int SongGenerator::progressInSeconds() {
    if (_songSpec == nullptr) {
        return 0;
    }
    if (_pattern == nullptr) {
        return _songSpec->lengthInSeconds();
    }

    int totalTicks = _ticksPlayedInEarlierPatterns + _patternGenerator.ticksPlayed();

    return totalTicks * SAMPLES_PER_TICK / SAMPLERATE;
}

int SongGenerator::outputLevel() {
    return (_pattern == nullptr || _paused) ? 0 : _patternGenerator.outputLevel();
}

int SongGenerator::addSamples(Sample* buf, int maxSamples) {
    int totalAdded = 0;

    if (_pattern == nullptr) {
        // We're done
        return totalAdded;
    }

    if (_paused) {
        // Don't actually add any samples but signal that buffer is fully filled (with silence)
        return maxSamples;
    }

    do {
        totalAdded += _patternGenerator.addSamples(buf + totalAdded, maxSamples - totalAdded);
        if (totalAdded < maxSamples) {
            moveToNextPattern();
            if (_pattern == nullptr) {
                // We're done
                return totalAdded;
            }
            startPattern(false);
        }
    } while (totalAdded < maxSamples);

    return totalAdded;
}

//--------------------------------------------------------------------------------------------------
// Music Handler

MusicHandler::MusicHandler() {
    _readP = _buffer;
    _headP = _readP + 1;
    _zeroP = nullptr;
    _endP = _buffer + SOUND_MUSIC_BUFFERSIZE;
}

void MusicHandler::play(const TuneSpec* tuneSpec) {
    _tuneGenerator.setTuneSpec(tuneSpec);
    _zeroP = nullptr;
}

void MusicHandler::play(const SongSpec* songSpec, bool loop) {
    _songGenerator.setSongSpec(songSpec, loop);
    _zeroP = nullptr;
}

int MusicHandler::outputLevel() {
    return _tuneGenerator.outputLevel() + _songGenerator.outputLevel();
}

void MusicHandler::update() {
    int16_t* targetHeadP = _readP; // Copy as its continuously changing

    while (_headP != targetHeadP) {
        // The maximum number of samples that can be added without wrapping
        int maxSamples = (int)((_headP < targetHeadP) ? targetHeadP - _headP : _endP - _headP);
        bool addedZeros = false;

        if (!_tuneGenerator.isDone()) {
            clearBuffer(_headP, maxSamples);
            addedZeros = true;
            _tuneGenerator.addSamples(_headP, maxSamples);
        }
        if (!_songGenerator.isDone()) {
            if (!addedZeros) {
                clearBuffer(_headP, maxSamples);
                addedZeros = true;
            }
            _songGenerator.addSamples(_headP, maxSamples);
        }
        if (!addedZeros) {
            if (_zeroP == nullptr) {
                _zeroP = _headP;
            } else if (_zeroP == _headP) {
                // Buffer contains only zeroes. No need to set any.
                addedZeros = true;
                _zeroP += maxSamples; // Move it headP
            } else if (_zeroP > _headP) {
                // Stop once buffer contains only zeroes
                maxSamples = min(maxSamples, (int)(_zeroP - _headP));
            }
            if (!addedZeros) {
                clearBuffer(_headP, maxSamples);
            }
        }
        _headP += maxSamples;

        if (_headP == _endP) {
            // Reached end of cyclic buffer. Continue at the beginning.
            _headP = _buffer;
        }
    }
}

#ifndef STANDALONE
  } // Namespace
#endif
