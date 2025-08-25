#include "raylib.h"
#include "const.h"
#include "beat.h"
#include <stdio.h>

int main() {
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);

  CreateBeat(1);
  CreateBeat(2);
  CreateBeat(3);
  CreateBeat(4);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    // left partition
    DrawRectangleLines(0, 0, screenWUnit, screenHeight, WHITE);
    // right partition
    DrawRectangleLines(screenWUnit * 5, 0, screenWUnit, screenHeight, WHITE);

    // DrawRectangleLines(screenWUnit * 1, 0, screenWUnit, screenHeight, BLUE);
    // DrawRectangleLines(screenWUnit * 2, 0, screenWUnit, screenHeight, BLUE);
    // DrawRectangleLines(screenWUnit * 3, 0, screenWUnit, screenHeight, BLUE);
    // DrawRectangleLines(screenWUnit * 4, 0, screenWUnit, screenHeight, BLUE);
    DrawRectangleLines(screenWUnit, 0, screenWUnit * 4, headerHeight, BLUE);
    DrawRectangleLines(screenWUnit, screenHeight - headerHeight, screenWUnit * 4, headerHeight, BLUE);

    UpdateBeats();
    DrawBeats();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}