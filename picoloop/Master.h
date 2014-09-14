#ifndef __MASTER____
#define __MASTER____
#include <SDL/SDL.h>

#define DEFAULTFREQ      44100
#define DEFAULTSAMPLES   512
#define DEFAULTCHANNELS  2
#define DEFAULTPOLYPHONY 16
#define DEFAULTFORMAT    AUDIO_S16

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_DEPTH	16
#define WAVETABLE_SIZE  1024*16
//#define WAVETABLE_SIZE  128
#define FPU 0

//#define MAX_PATTERN_BY_PROJECT 256
//#define MAX_PATTERN_BY_PROJECT 16
#define MAX_PATTERN_BY_PROJECT 256

#define TRACK_MAX 4

#define BUFFER_FRAME DEFAULTSAMPLES
//#define DEFAULT_FREQ 44100


//INTERNAL

#define FILTER_TYPE_LOWPASS   0
#define FILTER_TYPE_BANDPASS  1
#define FILTER_TYPE_HIPASS    2

#define FILTER_ALGO_NOFILTER  0
#define FILTER_ALGO_BIQUAD    1
#define FILTER_ALGO_AMSYNTH   2


#endif
