#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include "beat.h"
extern Texture2D charTexture1;

void loadAssets();
void unloadAssets();
Texture2D getArrowTexture(BEAT_ARROW arrow);

#endif // !ASSETS_H