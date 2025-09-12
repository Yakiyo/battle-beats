#include "assets.h"
#include "beat.h"
#include "const.h"
#include "raygui.h"
#include "raylib.h"

int _streak;
int _score;

// should be called when starting a new game or resetting
void resetCounters() {
  _streak = 0;
  _score = 0;
}

// internal function to draw the game board
void _drawBoard(Beatmap* beatmap, Music* music);

void drawSingleplayerPage(Beatmap* beatmap, Music* music) {
  UpdateMusicStream(*music);
  // if (!IsMusicStreamPlaying(*music)) {
  //   // if music is paused we dont do any updates to beats
  // TODO: draw a paused screen
  //   return;
  // }
  _drawBoard(beatmap, music);

  // Beat v = beatmap->beats.data[0];
  // v.posX = screenWidth / 2;
  // v.posY = screenHeight / 2;
  // drawBeat(&v);
  // // for (int i = 0; i < beatmap->beats.length; i++) {
  // //   drawBeat(&beatmap->beats.data[i]);
  // // }
}

void _drawBoard(Beatmap* beatmap, Music* music) {
  Texture2D up = getArrowTexture(BEAT_UP);
  Texture2D down = getArrowTexture(BEAT_DOWN);
  Texture2D left = getArrowTexture(BEAT_LEFT);
  Texture2D right = getArrowTexture(BEAT_RIGHT);

  //  draw streak counter
  if (_streak > 99) _streak = 99;  // cap streak at 99 for display purposes
  _streak = 45;
  int streak_10 = _streak / 10;
  int streak_1 = _streak % 10;

  DrawRectangleLinesEx((Rectangle){screenWidth / 6 + 20 + 60 * 0, 20, 60, 60}, 5, WHITE);
  DrawRectangleLinesEx((Rectangle){screenWidth / 6 + 20 + 60 * 1, 20, 60, 60}, 5, WHITE);
  DrawRectangleLinesEx((Rectangle){screenWidth / 6 + 20 + 60 * 2, 20, 60, 60}, 5, WHITE);

  DrawText("x", screenWidth / 6 + 20 + 20, 30, 50, WHITE);
  DrawText(TextFormat("%d", streak_10), screenWidth / 6 + 20 + 60 * 1 + 20, 30, 50, WHITE);
  DrawText(TextFormat("%d", streak_1), screenWidth / 6 + 20 + 60 * 2 + 20, 30, 50, WHITE);

  // end - streak counter

  // board outline
  DrawRectangleLinesEx((Rectangle){screenWidth / 6, 100, screenWidth / 6 * 4, screenHeight}, 10, WHITE);

  // 4 top to bottom lines
  for (int i = 1; i <= 4; i++) {
    DrawLineEx((Vector2){screenWidth / 6 * (i + 0.5), 100}, (Vector2){screenWidth / 6 * (i + 0.5), screenHeight - 15 - up.height}, 5, WHITE);
  }

  // draw a circular border around the bottom arrow textures
  for (int i = 1; i <= 4; i++) {
    DrawRing((Vector2){screenWidth / 6 * (i + 0.5), screenHeight - (up.height / 2) - 15}, up.width / 2, up.width / 2 + 5, 0, 360, 100, WHITE);
  }

  // draw the arrow textures
  DrawTexture(up, screenWidth / 6 * (1 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);
  DrawTexture(down, screenWidth / 6 * (2 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);
  DrawTexture(left, screenWidth / 6 * (3 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);
  DrawTexture(right, screenWidth / 6 * (4 + 0.5) - up.width / 2, screenHeight - up.height - 15, WHITE);

  // draw a line above the arrows
  DrawLineEx((Vector2){screenWidth / 6, screenHeight - up.height - 20}, (Vector2){screenWidth / 6 * 5, screenHeight - up.height - 20}, 5, WHITE);

  // draw a indicator line for music progress
  float progress = GetMusicTimePlayed(*music) / GetMusicTimeLength(*music);
  if (progress > 1.0f) progress = 1.0f;  // cap at 100%
  DrawRectangle(screenWidth / 6 * 2, 50, screenWidth / 6 * 2, 12, LIGHTGRAY);
  DrawRectangle(screenWidth / 6 * 2, 50, (int)(progress * screenWidth / 6 * 2), 12, MAROON);
  DrawCircle((screenWidth / 6 * 2) + (progress * screenWidth / 6 * 2), 50 + 6, 10, MAROON);

  // end - indicator line

  // use a button to show score, auto recenters it so less work for us
  GuiButton((Rectangle){screenWidth / 6 * 4, 20, screenWidth / 6, 80}, TextFormat("Score: %d", _score));
}