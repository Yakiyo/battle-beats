#ifndef BEAT_H
#define BEAT_H

#include "external/vec.h"

typedef enum BEAT_TYPE {
  BEAT_TAP,
  BEAT_HOLD,
} BEAT_TYPE;

typedef enum BEAT_ARROW {
  BEAT_UP = 0,
  BEAT_DOWN = 1,
  BEAT_LEFT = 2,
  BEAT_RIGHT = 3,
} BEAT_ARROW;

typedef struct Beat {
  BEAT_TYPE type;
  BEAT_ARROW arrow;
  int time;
  int end_time;  // only used for HOLD type
  int posX;
  int posY;
} Beat;

typedef struct Beatmap {
    // location of the music file
    char* music;
    // duration of music in seconds (based on the beatmap file, not the actual audio file)
    int duration;
    // array of beats
    vec_t(Beat) beats;
} Beatmap;

Beatmap* readBeatmap(const char* filename);
// debugging function
void _print_beat(Beat* beat);

void drawBeat(Beat* beat);

#endif  // !BEAT_H