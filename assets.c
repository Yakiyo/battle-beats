#include "raylib.h"
#include "beat.h"

Texture2D charTexture1;
Texture2D arrowUp;
Texture2D arrowDown;
Texture2D arrowLeft;
Texture2D arrowRight;
Texture2D arrowA;
Texture2D arrowW;
Texture2D arrowS;
Texture2D arrowD;

void loadAssets() {
  Image charImage1 = LoadImage("asset/guiter-player-1.png");
  charTexture1 = LoadTextureFromImage(charImage1);

  Image a_right_i = LoadImage("asset/beat-right.png");
  Image a_left_i = LoadImage("asset/beat-left.png");
  Image a_up_i = LoadImage("asset/beat-up.png");
  Image a_down_i = LoadImage("asset/beat-down.png");

  Image a_a_i = LoadImage("asset/beat-a.png");
  Image a_w_i = LoadImage("asset/beat-w.png");
  Image a_s_i = LoadImage("asset/beat-s.png");
  Image a_d_i = LoadImage("asset/beat-d.png");

  // 100x100 images (50px radius)
  ImageResize(&a_right_i, 100, 100);
  ImageResize(&a_left_i, 100, 100);
  ImageResize(&a_up_i, 100, 100);
  ImageResize(&a_down_i, 100, 100);

  ImageResize(&a_a_i, 100, 100);
  ImageResize(&a_w_i, 100, 100);
  ImageResize(&a_s_i, 100, 100);
  ImageResize(&a_d_i, 100, 100);

  arrowRight = LoadTextureFromImage(a_right_i);
  arrowLeft = LoadTextureFromImage(a_left_i);
  arrowUp = LoadTextureFromImage(a_up_i);
  arrowDown = LoadTextureFromImage(a_down_i);

  arrowA = LoadTextureFromImage(a_a_i);
  arrowW = LoadTextureFromImage(a_w_i);
  arrowS = LoadTextureFromImage(a_s_i);
  arrowD = LoadTextureFromImage(a_d_i);
  
  // unload images from RAM after loading textures
  UnloadImage(a_right_i);
  UnloadImage(a_left_i);
  UnloadImage(a_up_i);
  UnloadImage(a_down_i);
  
  UnloadImage(a_a_i);
  UnloadImage(a_w_i);
  UnloadImage(a_s_i);
  UnloadImage(a_d_i);

  UnloadImage(charImage1);
}

void unloadAssets() {
  UnloadTexture(charTexture1);
  UnloadTexture(arrowUp);
  UnloadTexture(arrowDown);
  UnloadTexture(arrowLeft);
  UnloadTexture(arrowRight);
  UnloadTexture(arrowA);
  UnloadTexture(arrowW);
  UnloadTexture(arrowS);
  UnloadTexture(arrowD);
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

Texture2D getArrowTextureAlpha(BEAT_ARROW arrow) {
  switch (arrow) {
    case BEAT_UP:
      return arrowW;
    case BEAT_DOWN:
      return arrowS;
    case BEAT_LEFT:
      return arrowA;
    case BEAT_RIGHT:
      return arrowD;
    default:
      return arrowW;  // fallback
  }
}