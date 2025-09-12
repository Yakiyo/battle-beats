#ifndef SINGLEPLAYER_H
#define SINGLEPLAYER_H

#include "raylib.h"
#include "beat.h"

void drawSingleplayerPage(Beatmap* beatmap);
void resetCounters();
void prepare_game_singleplayer(char* beatFile, Beatmap* currentBeatmap);

#endif // !SINGLEPLAYER_H