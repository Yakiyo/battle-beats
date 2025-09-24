#include "beat.h"

#include <stdio.h>

#include "assets.h"
#include "external/vec.h"
#include "raylib.h"

Beatmap readBeatmap(const char* filename) {
  Beatmap beatmap;
  beatmap.music = NULL;
  char filepath[512];
  snprintf(filepath, sizeof(filepath), ".\\beatmaps\\%s", filename);

  FILE* file = fopen(filepath, "r");
  if (!file) {
    printf("Failed to open beatmap file %s\n", filepath);
    return beatmap;
  }
  char music_path[256];
  fgets(music_path, 256, file);
  beatmap.music = strdup(music_path);
  int len = strlen(beatmap.music);
  if (len > 0 && beatmap.music[len - 1] == '\n') {
    beatmap.music[len - 1] = '\0';  // Remove newline character
  }

  fscanf(file, "%d\n", &beatmap.duration);
  vec_init(&beatmap.beats);
  int time_start, time_end, arrow;
  while (fscanf(file, "%d %d %d", &time_start, &time_end, &arrow) == 3) {
    Beat beat;
    beat.type = (time_end == 0) ? BEAT_TAP : BEAT_HOLD;
    beat.time = time_start;
    beat.end_time = time_end;
    beat.arrow = (BEAT_ARROW)arrow;
    beat.posX = 0;  // Default position
    beat.posY = 0;  // Default position
    vec_push(&beatmap.beats, beat);
  }

  fclose(file);
  return beatmap;
}

void _print_beat(Beat* beat) {
  printf(
      "Beat - Type: %s, Arrow: %d, Time: %d, End Time: %d, PosX: %d, PosY: "
      "%d\n",
      (beat->type == BEAT_TAP) ? "TAP" : "HOLD", beat->arrow, beat->time,
      beat->end_time, beat->posX, beat->posY);
}

void drawBeat(Beat* beat, int alpha) {
  Texture2D texture;
  if (alpha == 0)
    texture = getArrowTexture(beat->arrow);
  else
    texture = getArrowTextureAlpha(beat->arrow);

  DrawTexture(texture, beat->posX - texture.width / 2, beat->posY - texture.height / 2, WHITE);
}

int getKey(BEAT_ARROW arrow) {
  switch (arrow) {
    case BEAT_LEFT:
      return KEY_LEFT;
    case BEAT_DOWN:
      return KEY_DOWN;
    case BEAT_UP:
      return KEY_UP;
    case BEAT_RIGHT:
      return KEY_RIGHT;
  }
}

int getKeyAlpha(BEAT_ARROW arrow) {
  switch (arrow) {
    case BEAT_LEFT:
      return KEY_A;
    case BEAT_DOWN:
      return KEY_S;
    case BEAT_UP:
      return KEY_W;
    case BEAT_RIGHT:
      return KEY_D;
  }
}