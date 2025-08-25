#ifndef BEAT_H
#define BEAT_H

#include "vec.h"

// Beat struct definition
typedef struct Beat {
  // 0 means its yet to come
  // 1 means it is being drawn
  // 2 means we passed it
  int passed;
  int time;
  int slot; // 1-4
  int posX;
  int posY;
} Beat;

extern Vec(Beat) beats;
extern int score;

// Functions
int startX(int slot);
void CreateBeat(int slot, int time);
void UpdateBeats(int timeElapsed);
void DrawBeats();
int ReadBeatMapFile();
void LogBeats();

#endif // BEAT_H
