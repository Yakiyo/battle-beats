#include <stdio.h>

#include "assets.h"
#include "beat.h"
#include "const.h"
#include "page.h"
#include "raygui.h"
#include "raylib.h"

Music music;

const int _beat_speed = 4;  // pixels per frame

int _streak;
int _score;

Color GUITER_BODY = (Color){170, 112, 79, 255};
Color GUITER_BORDER = (Color){101, 67, 33, 255};

const int boardWidth = 800;

// should be called when starting a new game or resetting
void resetCounters() {
  _streak = 0;
  _score = 0;
}

// internal function to draw the game board
void _drawBoard(Beatmap* beatmap);
// internal function to draw the top stats section
// includes streak, score and a music progress bar
void _drawStatSection();

void drawSingleplayerPage(Beatmap* beatmap) {
  UpdateMusicStream(music);
  // TODO: create pause screen maybe?

  // draw the board
  _drawBoard(beatmap);

  // draw all beats
  int i;
  Beat* beat;
  vec_foreach_ptr(&beatmap->beats, beat, i) {
    int y = beat->posY;
    if (y > screenHeight - 50 - 20) {
      // beat crossed the bottom without being hit, so its a miss
      if (beat->pressed == 0) {
        // Missed the beat
        printf("Miss! Streak broken. beat time %d y %d\n", beat->time, y);
        _streak = 0;
      }
    }
    // if the beat is out of screen, skip drawing it
    if (y < 100 || y > (screenHeight - 50 - 20)) continue;
    drawBeat(beat);
    int gap = (screenHeight - 50 - 20) - y;
    if (gap <= 100) {
      int key = getKey(beat->arrow);
      if (IsKeyPressed(key)) {
        beat->pressed = 1;
        if (gap <= 60) {
          // perfect hit
          _streak++;
          if (_streak > 99) _streak = 99;  // cap streak at 99
          _score += 300;
          printf("Perfect! Streak: %d\n", _streak);
        } else {
          // good hit
          // does not increase streak
          // but does not break it either
          _score += 100;
        }
      }
    }
  };
  // draw top stats section
  _drawStatSection();
  // update beats
  vec_foreach_ptr(&beatmap->beats, beat, i) {
    // move the beat down by _beat_speed pixels
    beat->posY += _beat_speed;
  };
}

void _drawStatSection() {
  int boardLeft = (screenWidth - boardWidth) / 2;
  // cover up the top portion of the screen to make it look like beats are coming from underneath
  DrawRectangle(boardLeft, 0, boardWidth, 100, BLACK);

  //  draw streak counter
  {
    if (_streak > 99) _streak = 99;  // cap streak at 99 for display purposes
    int streak_10 = _streak / 10;
    int streak_1 = _streak % 10;

    DrawRectangleLinesEx((Rectangle){screenWidth / 6 + 20 + 60 * 0, 20, 60, 60}, 5, WHITE);
    DrawRectangleLinesEx((Rectangle){screenWidth / 6 + 20 + 60 * 1, 20, 60, 60}, 5, WHITE);
    DrawRectangleLinesEx((Rectangle){screenWidth / 6 + 20 + 60 * 2, 20, 60, 60}, 5, WHITE);

    DrawText("x", screenWidth / 6 + 20 + 20, 30, 50, WHITE);
    DrawText(TextFormat("%d", streak_10), screenWidth / 6 + 20 + 60 * 1 + 20, 30, 50, WHITE);
    DrawText(TextFormat("%d", streak_1), screenWidth / 6 + 20 + 60 * 2 + 20, 30, 50, WHITE);
  }

  // draw a indicator line for music progress
  {
    float progress = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
    if (progress > 1.0f) progress = 1.0f;  // cap at 100%
    DrawRectangle(screenWidth / 6 * 2, 50, screenWidth / 6 * 2, 12, GUITER_BODY);
    DrawRectangle(screenWidth / 6 * 2, 50, (int)(progress * screenWidth / 6 * 2), 12, GUITER_BODY);
    DrawCircle((screenWidth / 6 * 2) + (progress * screenWidth / 6 * 2), 50 + 6, 10, GUITER_BODY);
  }

  // use a button to show score, auto recenters it, so less work for us
  GuiButton((Rectangle){screenWidth / 6 * 4, 20, screenWidth / 6, 80}, TextFormat("Score: %d", _score));
}

void _drawBoard(Beatmap* beatmap) {
  int boardLeft = (screenWidth - boardWidth) / 2;
  int boardRight = boardLeft + boardWidth;
  int boardSegment = boardWidth / 4;

  Texture2D up = getArrowTexture(BEAT_UP);
  Texture2D down = getArrowTexture(BEAT_DOWN);
  Texture2D left = getArrowTexture(BEAT_LEFT);
  Texture2D right = getArrowTexture(BEAT_RIGHT);

  // end - streak counter

  // board color
  DrawRectangle(boardLeft, 100, boardWidth, screenHeight, GUITER_BODY);

  // board outline
  DrawRectangleLinesEx((Rectangle){boardLeft, 100, boardWidth, screenHeight}, 10, GUITER_BORDER);

  // 4 top to bottom lines
  for (int i = 0; i < 4; i++) {
    DrawLineEx((Vector2){boardLeft + boardSegment * (i + 0.5), 100}, (Vector2){boardLeft + boardSegment * (i + 0.5), screenHeight - 15 - up.height}, 5, WHITE);
  }

  // draw 2 horizontal lines to make it look like a guiter?
  int bodyHeight = screenHeight - 200 - 15;
  for (int i = 1; i <= 2; i++) {
    DrawLineEx((Vector2){boardLeft + 10, bodyHeight / 3 * i + 100}, (Vector2){boardLeft + boardWidth - 10, bodyHeight / 3 * i + 100}, 5, WHITE);
  }

  // draw a circular border around the bottom arrow textures
  for (int i = 0; i < 4; i++) {
    DrawRing((Vector2){boardLeft + boardSegment * (i + 0.5), screenHeight - (up.height / 2) - 20}, up.width / 2, up.width / 2 + 5, 0, 360, 100, WHITE);
  }

  // draw the arrow textures
  DrawTexture(up, boardLeft + boardSegment * 0.5 - up.width / 2, screenHeight - up.height - 20, WHITE);
  DrawTexture(down, boardLeft + boardSegment * 1.5 - up.width / 2, screenHeight - up.height - 20, WHITE);
  DrawTexture(left, boardLeft + boardSegment * 2.5 - up.width / 2, screenHeight - up.height - 20, WHITE);
  DrawTexture(right, boardLeft + boardSegment * 3.5 - up.width / 2, screenHeight - up.height - 20, WHITE);

  // draw a line above the arrows
  DrawLineEx((Vector2){boardLeft + 10, screenHeight - up.height - 25}, (Vector2){boardLeft + boardWidth - 10, screenHeight - up.height - 25}, 5, WHITE);
}

/**
 * Each beat needs to cross 960px on the board (50px from top to 1010px at bottom)
 * In 1 second, there are 60 frames, and beats move 4px per frame, so it takes a beat exactly
 * 4 seconds to cross the entire board.
 */

void prepare_game_singleplayer(char* beatFile, Beatmap* currentBeatmap) {
  int boardLeft = (screenWidth - boardWidth) / 2;
  int boardSegment = boardWidth / 4;
  // Load the beatmap
  if (beatFile == NULL || currentBeatmap == NULL) {
    printf("No beatmap selected!\n");
    setCurrentPage(PAGE_MENU);
    return;
  }
  // assign the positions of the beats based on their arrow type
  // and set their initial y position to be above the screen
  // split screen width into 4 equal parts for each arrow
  // and two more for the paddings on the sides
  int i;
  Beat* beat;
  vec_foreach_ptr(&currentBeatmap->beats, beat, i) {
    // set x position based on arrow type
    beat->posX = boardLeft + boardSegment * (beat->arrow + 0.5);
    // calculate y position based on time
    // the beat should reach the bottom (y=1010) at its time
    // assuming a beat that has time=0, it should be at y=1010. for each
    // second later, it should be 240px higher (4px per frame * 60 frames)
    int y;
    y = (screenHeight - 50) - (beat->time * 60 * _beat_speed);
    beat->posY = y;
  }
  resetCounters();
  // Initialize music
  music = LoadMusicStream(currentBeatmap->music);
  // Start playing music
  PlayMusicStream(music);
}