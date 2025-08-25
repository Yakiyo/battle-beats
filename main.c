#include "raylib.h"
#include "const.h"
#include "beat.h"
#include "beatmap.h"
#include <time.h>
#include <stdio.h>

int main() {
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);

  ReadBeatMapFile();

  CreateBeat(1);
  CreateBeat(2);
  CreateBeat(3);
  CreateBeat(4);

  time_t start = time(NULL);
  int is_enter = 0;
  while (!WindowShouldClose()) {
    int seconds_passed = difftime(time(NULL), start);
    BeginDrawing();
    ClearBackground(BLACK);
    // left partition
    DrawRectangleLines(0, 0, screenWUnit, screenHeight, WHITE);
    // right partition
    DrawRectangleLines(screenWUnit * 5, 0, screenWUnit, screenHeight, WHITE);
    DrawRectangleLines(screenWUnit, 0, screenWUnit * 4, headerHeight, BLUE);

    UpdateBeats();
    DrawBeats();
    DrawRectangle(screenWUnit, screenHeight - headerHeight, screenWUnit * 4, headerHeight, GREEN);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}