#include "external/vec.h"

#ifndef BEATMAP_H
#define BEATMAP_H

void loadBeatmaps();
void unloadBeatmaps();

extern vec_str_t beatmap_files;

#endif // !BEATMAP_H