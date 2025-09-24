#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "beat.h"

void prepare_game_multiplayer(char* beatFile, Beatmap* currentBeatmap);
void drawMultiplayerPage(Beatmap* beatmap);

#endif // !MULTIPLAYER_H