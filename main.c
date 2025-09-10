#define RAYGUI_IMPLEMENTATION
#include <stdio.h>

#include "const.h"
#include "raygui.h"
#include "raylib.h"
#include "util.h"

PageType currentPage = PAGE_OPENING;
Texture2D charTexture1;

float _fader_timer = 0.0f;

void drawOpeningPage(int is_menu);

int main() {
  InitWindow(screenWidth, screenHeight, "Beat Game");
  SetTargetFPS(60);

  Image charImage1 = LoadImage("asset/guiter-player-1.png");
  

  charTexture1 = LoadTextureFromImage(charImage1);

  // unload images from RAM after loading textures
  UnloadImage(charImage1);

  // Global GUI styles
  GuiSetStyle(DEFAULT, TEXT_SIZE, 50);
  GuiSetStyle(DEFAULT, TEXT_SPACING, 2);

  // Transparent backgrounds

  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x00000000);
  GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x00000000);
  GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x00000000);

  GuiSetStyle(DEFAULT, BORDER_WIDTH, 0);  // hide outline

  // Text colors

  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);   // White
  GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0xFF69B4FF);  // pink hover
  GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0xFF69B4FF);  // pink press

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int quit = 0;
    switch (currentPage) {
      case PAGE_OPENING:
      case PAGE_MENU:
        drawOpeningPage(currentPage == PAGE_MENU ? 1 : 0);
        break;
      case QUIT_GAME:
        quit = 1;
        break;
    }

    EndDrawing();
    if (quit) break;
  }

  UnloadTexture(charTexture1);

  CloseWindow();
  return 0;
}

void drawOpeningPage(int is_menu) {
  int titleFontSize = 100;
  int textFontSize = 50;
  char* title = "Battle Beats";
  int titleWidth = MeasureText(title, titleFontSize);

  DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 4,
           titleFontSize, PINK);

  if (!is_menu) {
    // change alpha over time which makes the text fade in and out
    _fader_timer += GetFrameTime();
    float alpha =
        (sinf(_fader_timer * 2.0f) + 1.0f) * 0.5f * 255;  // 2.0f is speed

    char* text = "Press ENTER to continue";
    DrawText(text, (screenWidth - MeasureText(text, textFontSize)) / 2,
             // light gray color
             screenHeight / 2, textFontSize, (Color){200, 200, 200, alpha});

    if (IsKeyPressed(KEY_ENTER)) {
      currentPage = PAGE_MENU;
      _fader_timer = 0.0f;  // reset timer for next use
    }
  } else {
    int buttonWidth = 500;
    int buttonHeight = 100;
    int b_singleplayer = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2, screenHeight / 2 - 60, buttonWidth, buttonHeight},
        "Singleplayer");

    int b_multiplayer = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2, screenHeight / 2 - 60 + 100, buttonWidth, buttonHeight},
        "Multiplayer");
    int b_generator = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2, screenHeight / 2 - 60 + 200, buttonWidth, buttonHeight},
        "Beatmap Generator");
    
    int b_quit = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2, screenHeight / 2 - 60 + 300, buttonWidth, buttonHeight},
        "Quit Game");

    DrawTexture(charTexture1, screenWidth / 5 - charTexture1.width / 2,
                screenHeight / 2 - charTexture1.height / 2, WHITE);


    if (b_singleplayer) {
      currentPage = PAGE_SINGLEPLAYER;
    } else if (b_multiplayer) {
      currentPage = PAGE_MULTIPLAYER;
    } else if (b_generator) {
      currentPage = PAGE_GENERATOR;
    } else if (b_quit) {
      currentPage = QUIT_GAME;
    }
  }
}