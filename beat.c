#include<stdio.h>
#include "const.h"
#include "vec.h"
#include "raylib.h"


int startX(int slot) {
  return (screenWUnit * slot) + 40 + beatRadius;
}

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

Vec(Beat) beats;

void CreateBeat(int slot, int time) {
  int x = startX(slot);
  // int y = headerHeight + beatRadius;
  Beat beat;
  beat.slot = slot;
  beat.posX = x;
  beat.posY = 0;
  beat.time = time;
  vec_push(Beat, beats, beat);
}

void UpdateBeats(int timeElapsed) {
  for (int i = 0; i < vec_size(beats); i++) {
    Beat *beat = &vec_A(beats, i);
    if (beat->passed == 2) {
      // printf("Passing beat %d\n", i);
      continue;
    }
    // this beat is yet to come so we dont need to update it
    if (beat->time > timeElapsed) {
      // printf("Skipping beat %d\n", i);
      beat->passed = 0;
      continue;
    }
    beat->passed = 1;
    beat->posY += beatSpeed;

    if (beat->posY > screenHeight - headerHeight) {
      // reset to top
  
      // beat->posY = headerHeight + beatRadius;
      // it goes 480 pixels. so 80 pixels per second is 6 seconds of total traversel time
      beat->passed = 2;
    }
  }
}

void DrawBeats() {
  for (int i = 0; i < vec_size(beats); i++) {
    Beat beat = vec_A(beats, i);
    if (beat.passed != 1) continue;
    DrawCircle(beat.posX, beat.posY, beatRadius, BLUE);
  }
}

int ReadBeatMapFile() {
    int total_time = 0;
    FILE *file = fopen("beatmap.txt", "r");
    if (!file) {
        perror("Failed to open beatmap file");
        return 0;
    }
    fscanf(file, "%d", &total_time);
    int time, slot;
    while (fscanf(file, "%d %d", &time, &slot) == 2) {
        CreateBeat(slot, time);
    }
    
    fclose(file);
    return total_time;
}

void LogBeats() {
  for(int i = 0; i < vec_size(beats); i++) {
    Beat beat = vec_A(beats, i);
    printf("Beat %d: slot=%d, time=%d, pos=(%d,%d), passed=%d\n", i, beat.slot, beat.time, beat.posX, beat.posY, beat.passed);
  }
}