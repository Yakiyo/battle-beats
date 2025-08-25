#include<stdio.h>
#include "raylib.h"
#include "beat.h"
#include "const.h"

void HandleInput() {
  int key = GetKeyPressed();
  int slot = 0;
  switch (key)
  {
  case KEY_LEFT:
    slot = 1;
    break;
  case KEY_UP:
    slot = 2;
    break;
  case KEY_RIGHT:
    slot = 3;
    break;
  case KEY_DOWN:
    slot = 4;
    break;
  default:
    return;
  }
  printf("Pressed key %d\n", key);
  for (int i = 0; i < vec_size(beats); i++) {
    Beat *beat = &vec_A(beats, i);
    if (beat->passed != 1 || beat->slot != slot) continue;
    int y = beat->posY;
    // center of circle is inside the footer so its a perfect hit
    // if (y >= (screenHeight - headerHeight)) {
    //   // Hit detected
    //   score += 100;
    //   beat->passed = 2;  // Mark as passed
    //   printf("Hit detected on slot %d at y=%d\n", slot, y);
    // } else if (y >= (screenHeight - headerHeight + beatRadius)) {
    //   // Missed
    //   beat->passed = 2;  // Mark as passed
    //   printf("Missed on slot %d at y=%d\n", slot, y);
    // }
    if (y >= (screenHeight - (headerHeight + beatRadius))) {
      if (y >= (screenHeight - headerHeight)) {
        // Perfect hit
        score += 100;
      } else {
        // Good hit
        score += 50;
      }
      beat->passed = 2;  // Mark as passed
      printf("hit on slot %d at y=%d\n", slot, y);
    }
  }
}