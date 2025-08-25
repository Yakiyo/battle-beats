#include<stdio.h>
#include "const.h"
#include "vec.h"
#include "raylib.h"


int startX(int slot) {
  return (screenWUnit * slot) + 20 + beatRadius;
}

typedef struct Beat {
  int slot; // 1-4
  int posX;
  int posY;
} Beat;

Vec(Beat) beats;


void CreateBeat(int slot) {
  int x = startX(slot);
  int y = headerHeight + beatRadius;
  Beat beat;
  beat.slot = slot;
  beat.posX = x;
  beat.posY = y;
  vec_push(Beat, beats, beat);
}

// void UpdateBeat(int index, int newY) {
//   if (index < 0 || index >= vec_size(beats)) return; // out of bounds
//   vec_A(beats, index).posY = newY;
// }

void UpdateBeats() {
  for (int i = 0; i < vec_size(beats); i++) {
    Beat *beat = &vec_A(beats, i);
    beat->posY += beatSpeed;

    if (beat->posY > screenHeight - headerHeight) {
      // reset to top
  
      beat->posY = headerHeight + beatRadius;
    }
  }
}

void DrawBeats() {
  
  for (int i = 0; i < vec_size(beats); i++) {
    Beat beat = vec_A(beats, i);
    DrawCircle(beat.posX, beat.posY, beatRadius, BLUE);
  }
}