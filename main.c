#define RAYGUI_IMPLEMENTATION
#include<stdio.h>
#include "raylib.h"
#include "raygui.h"
#include "const.h"

int main() {
    InitWindow(SCREENWIDTH, SCREENHEIGHT, TITLE);
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}