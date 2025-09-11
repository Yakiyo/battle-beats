#define RAYGUI_IMPLEMENTATION
#define SUPPORT_MODULE_RAUDIO
#define SUPPORT_FILEFORMAT_MP3
#include <stdio.h>

#include "const.h"
#include "dir.h"
#include "raygui.h"
#include "raylib.h"
#include "util.h"
#include "beat.h"

PageType currentPage = PAGE_OPENING;
Texture2D charTexture1;
char* beatFile = NULL;
Beatmap* currentBeatmap = NULL;
Music music;

float _fader_timer = 0.0f;

void drawOpeningPage(int is_menu);

void drawBeatSelectionPage(int is_multi);

void prepare_game_singleplayer();

void drawSingleplayerPage();

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int main() {
  InitWindow(screenWidth, screenHeight, "Beat Game");
  InitAudioDevice();
  SetTargetFPS(60);

  Image charImage1 = LoadImage("asset/guiter-player-1.png");
  Image a_right_i = LoadImage("asset/beat-right.png");
  Image a_left_i = LoadImage("asset/beat-left.png");
  Image a_up_i = LoadImage("asset/beat-up.png");
  Image a_down_i = LoadImage("asset/beat-down.png");

  // 100x100 images (50px radius)
  ImageResize(&a_right_i, 100, 100);
  ImageResize(&a_left_i, 100, 100);
  ImageResize(&a_up_i, 100, 100);
  ImageResize(&a_down_i, 100, 100);

  charTexture1 = LoadTextureFromImage(charImage1);

  // unload images from RAM after loading textures
  UnloadImage(charImage1);
  UnloadImage(a_right_i);
  UnloadImage(a_left_i);
  UnloadImage(a_up_i);
  UnloadImage(a_down_i);
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

  loadBeatmaps();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int quit = 0;
    switch (currentPage) {
      case PAGE_OPENING:
      case PAGE_MENU:
        drawOpeningPage(currentPage == PAGE_MENU ? 1 : 0);
        break;
      case BEAT_SELECTION_SINGLE:
      case BEAT_SELECTION_MULTI:
        drawBeatSelectionPage(currentPage == BEAT_SELECTION_MULTI ? 1 : 0);
        break;
      case PAGE_SINGLEPLAYER:
        drawSingleplayerPage();
        break;
      case QUIT_GAME:
        quit = 1;
        break;
    }
    EndDrawing();
    if (quit) break;
  }

  UnloadTexture(charTexture1);
  unloadBeatmaps();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void drawOpeningPage(int is_menu) {
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

    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      currentPage = PAGE_MENU;
      _fader_timer = 0.0f;  // reset timer for next use
    }
  } else {
    int buttonWidth = 500;
    int buttonHeight = 100;
    int b_singleplayer =
        GuiButton((Rectangle){(screenWidth - buttonWidth) / 2,
                              screenHeight / 2 - 60, buttonWidth, buttonHeight},
                  "Singleplayer");

    int b_multiplayer = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2,
                    screenHeight / 2 - 60 + 100, buttonWidth, buttonHeight},
        "Multiplayer");
    int b_generator = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2,
                    screenHeight / 2 - 60 + 200, buttonWidth, buttonHeight},
        "Beatmap Generator");

    int b_quit = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2,
                    screenHeight / 2 - 60 + 300, buttonWidth, buttonHeight},
        "Quit Game");

    DrawTexture(charTexture1, screenWidth / 5 - charTexture1.width / 2,
                screenHeight / 2 - charTexture1.height / 2, WHITE);

    if (b_singleplayer) {
      currentPage = BEAT_SELECTION_SINGLE;
    } else if (b_multiplayer) {
      currentPage = BEAT_SELECTION_MULTI;
    } else if (b_generator) {
      currentPage = PAGE_GENERATOR;
    } else if (b_quit) {
      currentPage = QUIT_GAME;
    }
  }
}

void drawBeatSelectionPage(int is_multi) {
  char* title = "Select a Beatmap";
  DrawText(title, (screenWidth - MeasureText(title, titleFontSize)) / 2, 100,
           titleFontSize, PINK);

  int buttons[beatmap_files.length];
  for (int i = 0; i < beatmap_files.length; i++) {
    char* name = strdup(beatmap_files.data[i]);
    char* dot = strrchr(name, '.');  // find last '.'

    if (dot && strcmp(dot, ".txt") == 0) {
      *dot = '\0';  // truncate extension
    }
    buttons[i] = GuiButton(
        (Rectangle){screenWidth / 4, 300 + i * 100, screenWidth / 2, 100},
        beatmap_files.data[i]);
  }

  for (int i = 0; i < beatmap_files.length; i++) {
    // if a button is pressed, find the file name for it and then proceed to
    // game page with it
    if (buttons[i]) {
      beatFile = beatmap_files.data[i];
      printf("Selected beatmap: %s\n", beatFile);
      // Here you would typically load the beatmap and transition to gameplay
      currentPage = is_multi ? PAGE_MULTIPLAYER : PAGE_SINGLEPLAYER;
      beatFile = beatmap_files.data[i];
      printf("Selected beatmap: %s\n", beatFile);
      currentBeatmap = readBeatmap(beatFile);
      if (currentBeatmap == NULL) {
        printf("Failed to load beatmap: %s\n", beatFile);
        currentPage = PAGE_MENU;  // go back to menu on failure
      }
      if (!is_multi) {
        prepare_game_singleplayer();
      }
      // TODO: use prepare game multiplayer when that is implemented

      // int i;
      // Beat* beat;
      // vec_foreach_ptr(&currentBeatmap->beats, beat, i) {
      //   _print_beat(beat);
      // }
      break;
    }
  }
}

// beat radius for singleplayer
const int beat_radius_sp = 50;

void prepare_game_singleplayer() {
  // Load the beatmap
  if (beatFile == NULL) {
    printf("No beatmap selected!\n");
    currentPage = PAGE_MENU;
    return;
  }

  // Initialize music
  music = LoadMusicStream(currentBeatmap->music);
  // Start playing music
  PlayMusicStream(music);
}


void drawSingleplayerPage() {
  DrawText("Singleplayer Mode", 100, 100, textFontSize, WHITE);
  DrawText(currentBeatmap->music, 100, 200, textFontSize, WHITE);

  UpdateMusicStream(music);
}
