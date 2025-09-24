#include "external/vec.h"

#ifndef BEATMAP_H
#define BEATMAP_H

void loadBeatmaps();
void unloadBeatmaps();
vec_str_t getMusicFiles();

extern vec_str_t beatmap_files;
extern vec_str_t music_files;

#endif // !BEATMAP_H