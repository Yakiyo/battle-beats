#include "raylib.h"
#include "const.h"
#include "beat.h"
#include <time.h>
#include <stdio.h>

int main() {
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);

  time_t start = time(NULL);
  int end = ReadBeatMapFile();

  LogBeats();
  while (!WindowShouldClose()) {
    int seconds_passed = difftime(time(NULL), start);

    if (seconds_passed > end + 3) {
      break;
    }
    BeginDrawing();
    ClearBackground(BLACK);

    // left partition
    DrawRectangleLines(0, 0, screenWUnit, screenHeight, WHITE);
    // right partition
    DrawRectangleLines(screenWUnit * 5, 0, screenWUnit, screenHeight, WHITE);
    // DrawRectangleLines(screenWUnit, 0, screenWUnit * 4, headerHeight, BLUE);

    UpdateBeats(seconds_passed);
    DrawBeats();
    DrawRectangle(screenWUnit, screenHeight - headerHeight, screenWUnit * 4, headerHeight, GREEN);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}