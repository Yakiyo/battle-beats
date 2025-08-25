#ifndef BEAT_H
#define BEAT_H

// Beat struct definition
typedef struct Beat {
    int slot; // 1-4
    int posX;
    int posY;
} Beat;

// Functions
int startX(int slot);
void CreateBeat(int slot);
void UpdateBeats();
void DrawBeats();

#endif // BEAT_H
