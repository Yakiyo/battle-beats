#ifndef BEATMAP_H
#define BEATMAP_H

typedef struct BeatInput {
    int time;
    int key;
} BeatInput;

void ReadBeatMapFile();

#endif // BEATMAP_H