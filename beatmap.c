#include<stdio.h>
#include "beat.h"
#include "vec.h"

typedef struct BeatInput {
    int time;
    int key;
} BeatInput;

Vec(BeatInput) beatInputs;

void ReadBeatMapFile() {
    FILE *file = fopen("beatmap.txt", "r");
    if (!file) {
        perror("Failed to open beatmap file");
        return;
    }

    BeatInput input;
    int time, key;
    while (fscanf(file, "%d %d", &time, &key) == 2) {
        
        // BeatInput input = { time, key };
        // vec_push(BeatInput, beatInputs, input);
    }
    
    fclose(file);
}

// void BeatMapToBeat() {
//     for (size_t i = 0; i < vec_size(beatInputs); i++) {
//         BeatInput input = vec_A(beatInputs, i);
//         CreateBeat(input.time, input.key);
//     }
// }