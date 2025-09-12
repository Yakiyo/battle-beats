#define RAYGUI_IMPLEMENTATION
#define SUPPORT_MODULE_RAUDIO
#define SUPPORT_FILEFORMAT_MP3
#include <stdio.h>

#include "assets.h"
#include "beat.h"
#include "const.h"
#include "dir.h"
#include "page.h"
#include "raygui.h"
#include "raylib.h"
#include "singleplayer.h"
#include "util.h"

char* beatFile = NULL;
Beatmap* currentBeatmap = NULL;
Music music;

void drawBeatSelectionPage(int is_multi);

void prepare_game_singleplayer();

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int main() {
  // borderless windowed mode
  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  InitWindow(screenWidth, screenHeight, "Beat Game");
  InitAudioDevice();
  SetTargetFPS(60);
  loadAssets();

  {
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
  }

  loadBeatmaps();

  // TODO: manually handle escape key. use SetExitKey(0) to disable raylib's
  // default escape key behavior
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int quit = 0;
    switch (getCurrentPage()) {
      case PAGE_OPENING:
      case PAGE_MENU:
        drawOpeningPage(getCurrentPage() == PAGE_MENU ? 1 : 0);
        break;
      case BEAT_SELECTION_SINGLE:
      case BEAT_SELECTION_MULTI:
        drawBeatSelectionPage(getCurrentPage() == BEAT_SELECTION_MULTI ? 1 : 0);
        break;
      case PAGE_SINGLEPLAYER:
        drawSingleplayerPage(currentBeatmap, &music);
        break;
      case QUIT_GAME:
        quit = 1;
        break;
    }
    EndDrawing();
    if (quit) break;
  }

  unloadAssets();
  unloadBeatmaps();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

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
      setCurrentPage(is_multi ? PAGE_MULTIPLAYER : PAGE_SINGLEPLAYER);
      beatFile = beatmap_files.data[i];
      printf("Selected beatmap: %s\n", beatFile);
      currentBeatmap = readBeatmap(beatFile);
      if (currentBeatmap == NULL) {
        printf("Failed to load beatmap: %s\n", beatFile);
        setCurrentPage(PAGE_MENU);  // go back to menu on failure
      }
      if (!is_multi) {
        prepare_game_singleplayer();
      }
      // TODO: use prepare game multiplayer when that is implemented
      break;
    }
  }
}

// beat radius for singleplayer
const int beat_radius_sp = 50;

void prepare_game_singleplayer() {
  // Load the beatmap
  if (beatFile == NULL || currentBeatmap == NULL) {
    printf("No beatmap selected!\n");
    setCurrentPage(PAGE_MENU);
    return;
  }
  // assign the positions of the beats based on their arrow type
  // and set their initial y position to be above the screen
  // split screen width into 4 equal parts for each arrow
  // and two more for the paddings on the sides
  int i;
  Beat* beat;
  vec_foreach_ptr(&currentBeatmap->beats, beat, i) {
    beat->posX = screenWidth / 6 * (beat->arrow + 1);
    beat->posY = beat_radius_sp;  // all beats start at the top portion of the screen
  }
  resetCounters();
  // Initialize music
  music = LoadMusicStream(currentBeatmap->music);
  // Start playing music
  PlayMusicStream(music);
}
