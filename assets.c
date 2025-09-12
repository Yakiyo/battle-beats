#include "raylib.h"
#include "beat.h"

Texture2D charTexture1;
Texture2D arrowUp;
Texture2D arrowDown;
Texture2D arrowLeft;
Texture2D arrowRight;

void loadAssets() {
  Image charImage1 = LoadImage("asset/guiter-player-1.png");
  charTexture1 = LoadTextureFromImage(charImage1);

  Image a_right_i = LoadImage("asset/beat-right.png");
  Image a_left_i = LoadImage("asset/beat-left.png");
  Image a_up_i = LoadImage("asset/beat-up.png");
  Image a_down_i = LoadImage("asset/beat-down.png");

  // 100x100 images (50px radius)
  ImageResize(&a_right_i, 100, 100);
  ImageResize(&a_left_i, 100, 100);
  ImageResize(&a_up_i, 100, 100);
  ImageResize(&a_down_i, 100, 100);

  arrowRight = LoadTextureFromImage(a_right_i);
  arrowLeft = LoadTextureFromImage(a_left_i);
  arrowUp = LoadTextureFromImage(a_up_i);
  arrowDown = LoadTextureFromImage(a_down_i);
  // unload images from RAM after loading textures
  UnloadImage(a_right_i);
  UnloadImage(a_left_i);
  UnloadImage(a_up_i);
  UnloadImage(a_down_i);
  UnloadImage(charImage1);
}

void unloadAssets() {
  UnloadTexture(charTexture1);
  UnloadTexture(arrowUp);
  UnloadTexture(arrowDown);
  UnloadTexture(arrowLeft);
  UnloadTexture(arrowRight);
}

Texture2D getArrowTexture(BEAT_ARROW arrow) {
  switch (arrow) {
    case BEAT_UP:
      return arrowUp;
    case BEAT_DOWN:
      return arrowDown;
    case BEAT_LEFT:
      return arrowLeft;
    case BEAT_RIGHT:
      return arrowRight;
    default:
      return arrowUp;  // fallback
  }
}