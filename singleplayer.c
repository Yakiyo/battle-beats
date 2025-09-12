#include "assets.h"
#include "beat.h"
#include "const.h"
#include "raylib.h"

void drawBoard();

void drawSingleplayerPage(Beatmap* beatmap, Music* music) {
  UpdateMusicStream(*music);
  // if (!IsMusicStreamPlaying(*music)) {
  //   // if music is paused we dont do any updates to beats
  //   // TODO: draw a paused screen
  //   return;
  // }
  drawBoard();
  // Beat v = beatmap->beats.data[0];
  // v.posX = screenWidth / 2;
  // v.posY = screenHeight / 2;
  // drawBeat(&v);
  // // for (int i = 0; i < beatmap->beats.length; i++) {
  // //   drawBeat(&beatmap->beats.data[i]);
  // // }
}

void drawBoard() {
  // draw the 4 arrows at the bottom of the screen
  Texture2D up = getArrowTexture(BEAT_UP);
  Texture2D down = getArrowTexture(BEAT_DOWN);
  Texture2D left = getArrowTexture(BEAT_LEFT);
  Texture2D right = getArrowTexture(BEAT_RIGHT);

  DrawLineEx((Vector2){screenWidth / 6, 0}, (Vector2){screenWidth / 6, screenHeight}, 10, WHITE);
  DrawRectangleLinesEx((Rectangle){screenWidth / 6, 0, screenWidth / 6 * 4, screenHeight}, 10, WHITE);

  for (int i = 1; i <= 4; i++) {
    DrawLineEx((Vector2){screenWidth / 6 * (i + 0.5), 100}, (Vector2){screenWidth / 6 * (i + 0.5), screenHeight - 15 - up.height}, 5, WHITE);
  }

  // draw a circular border around the bottom arrow textures
  for (int i = 1; i <= 4; i++) {
    DrawRing((Vector2){screenWidth / 6 * (i + 0.5), screenHeight - (up.height / 2) - 15}, up.width / 2, up.width / 2 + 5, 0, 360, 100, WHITE);
  }
  DrawTexture(up, screenWidth / 6 * (1 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);
  DrawTexture(down, screenWidth / 6 * (2 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);
  DrawTexture(left, screenWidth / 6 * (3 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);
  DrawTexture(right, screenWidth / 6 * (4 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);
}