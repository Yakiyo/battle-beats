#include <stdio.h>

#include "beat.h"
#include "const.h"
#include "dir.h"
#include "page.h"
#include "raygui.h"
#include "raylib.h"

typedef enum GENPAGE {
  GEN,
  END,
  PAUSE,
} GENPAGE;

GENPAGE _page = GEN;
int wrote_to_file = 0;
vec_t(Beat) _beats;
char* selected_music_file = NULL;
Music track;

char arrows[4] = {'^', 'v', '<', '>'};
// char arrows[4] = {'↑', '↓', '←', '→'};

void _resetState();
void write_file();

void generator_selection_page() {
  int buttons[music_files.length];
  char* title = "Select a Music File";

  DrawText(title, (screenWidth - MeasureText(title, titleFontSize)) / 2, 100,
           titleFontSize, PINK);

  for (int i = 0; i < music_files.length; i++) {
    char* name = strdup(music_files.data[i]);
    char* dot = strrchr(name, '.');  // find last '.'

    if (dot && strcmp(dot, ".mp3") == 0) {
      *dot = '\0';  // truncate extension
    }
    buttons[i] = GuiButton(
        (Rectangle){screenWidth / 4, 300 + i * 100, screenWidth / 2, 100},
        name);
  }

  for (int i = 0; i < music_files.length; i++) {
    // if a button is pressed, find the file name for it and then proceed to
    // game page with it
    if (buttons[i]) {
      _resetState();
      // sprintf(selected_music_file, "beatmaps/%s", music_files.data[i]);
      selected_music_file = music_files.data[i];
      setCurrentPage(PAGE_GENERATOR);
      printf("Selected music file: %s\n", selected_music_file);

      track = LoadMusicStream(TextFormat("beatmaps/%s", selected_music_file));
      track.looping = false;
      PlayMusicStream(track);
      vec_init(&_beats);
      _page = GEN;
      break;
    }
  }
}

void _resetState() {
  _page = GEN;
  wrote_to_file = 0;
  if (_beats.length > 0) vec_deinit(&_beats);
  if (selected_music_file) {
    UnloadMusicStream(track);
    selected_music_file = NULL;
  }
}

void generator_page() {
  char* title = "Music Generator Page";
  DrawText(title, (screenWidth - MeasureText(title, textFontSize)) / 2, 100, textFontSize, PINK);
  DrawText(selected_music_file, (screenWidth - MeasureText(selected_music_file, textFontSize)) / 2, 200, textFontSize, WHITE);
  UpdateMusicStream(track);
  DrawText("Press SPACE to play/pause music. Press ENTER to finish.", (screenWidth - MeasureText("Press SPACE to play/pause music. Press ENTER to return to menu.", textFontSize)) / 2, 300, textFontSize, WHITE);

  if (_page == END) {
    DrawText("Music generation finished.", (screenWidth - MeasureText("Music generation finished.", textFontSize)) / 2, 350, textFontSize, WHITE);

    if (wrote_to_file) {
      DrawText("File written to beatmaps folder", (screenWidth - MeasureText("File written to beatmaps folder", textFontSize)) / 2, 400, textFontSize, WHITE);
      DrawText("Press ENTER to return to menu.", (screenWidth - MeasureText("Press ENTER to return to menu.", textFontSize)) / 2, 450, textFontSize, WHITE);
      if (IsKeyPressed(KEY_ENTER)) {
        _resetState();
        setCurrentPage(PAGE_MENU);
        return;
      }
    } else {
      DrawText("Writing to file...", (screenWidth - MeasureText("Writing to file...", textFontSize)) / 2, 400, textFontSize, WHITE);
      write_file();
      wrote_to_file = 1;
    }
  }

  if (IsKeyPressed(KEY_SPACE)) {
    if (IsMusicStreamPlaying(track)) {
      PauseMusicStream(track);
      _page = PAUSE;
    } else {
      PlayMusicStream(track);
      _page = GEN;
    }
  }

  if (IsKeyPressed(KEY_ENTER)) {
    _page = END;
    PauseMusicStream(track);
    return;
  }

  if (!IsMusicStreamPlaying(track) && _page == GEN) {
    _page = END;
  }

  Color GUITER_BODY = (Color){170, 112, 79, 255};
  Color GUITER_BORDER = (Color){101, 67, 33, 255};
  float progress = GetMusicTimePlayed(track) / GetMusicTimeLength(track);
  if (progress > 1.0f) progress = 1.0f;

  DrawRectangle(320, screenHeight - 320, 1280, 20, GUITER_BODY);
  DrawRectangle(320, screenHeight - 320, 1280 * progress, 20, GUITER_BORDER);
  DrawCircle(320 + 1280 * progress, screenHeight - 310, 15, GUITER_BORDER);

  int i;
  Beat* beat;
  vec_foreach_ptr(&_beats, beat, i) {
    DrawText(TextFormat("%c", arrows[beat->arrow]), 1280 * (beat->time / (GetMusicTimeLength(track))) + 320, screenHeight - 300, 50, WHITE);
  };

  int key = GetKeyPressed();

  BEAT_ARROW arrow;
  switch (key) {
    case KEY_UP:
      arrow = BEAT_UP;
      break;
    case KEY_DOWN:
      arrow = BEAT_DOWN;
      /* code */
      break;
    case KEY_LEFT:
      arrow = BEAT_LEFT;
      /* code */
      break;
    case KEY_RIGHT:
      arrow = BEAT_RIGHT;
      /* code */
      break;
    default:
      arrow = -1;
      break;
  }
  if (arrow == -1) return;

  Beat new;
  new.type = BEAT_TAP;
  new.arrow = arrow;
  new.time = (int)(GetMusicTimePlayed(track));

  vec_push(&_beats, new);
}

void write_file() {
  char* mname = strdup(selected_music_file);
  char* dot = strrchr(mname, '.');  // find last '.'
  if (dot && strcmp(dot, ".mp3") == 0) {
    *dot = '\0';  // truncate extension
  }
  char* filename = TextFormat("beatmaps/%s.txt", mname);
  FILE* file = fopen(filename, "w");
  if (!file) {
    perror("Failed to open file");
    return;
  }

  fprintf(file, "beatmaps/%s\n", selected_music_file);
  fprintf(file, "%d\n", (int)(GetMusicTimeLength(track)));
  for (int i = 0; i < _beats.length; i++) {
    Beat* beat = &_beats.data[i];
    fprintf(file, "%d %d %d\n", beat->time, 0, (int)beat->arrow);
  }

  fclose(file);
}