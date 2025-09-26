#include <stdio.h>

#include "assets.h"
#include "beat.h"
#include "const.h"
#include "page.h"
#include "raylib.h"
#include "raygui.h"

Music _music;

const int beat_speed = 6;  // pixels per frame
int streakL;
int scoreL;
int streakR;
int scoreR;

Color _GUITER_BODY = (Color){170, 112, 79, 255};
Color _GUITER_BORDER = (Color){101, 67, 33, 255};

const int _boardWidth = 800;
int _music_unloaded = 1;

// should be called when starting a new game or resetting
void _resetCounters() {
  streakL = 0;
  scoreL = 0;
  streakR = 0;
  scoreR = 0;
  _music_unloaded = 1;
}

typedef enum MPPAGE {
  GAME,
  PAUSE,
  END,
} MPPAGE;

MPPAGE _currentMPPage = GAME;

vec_t(Beat) player1_beats;
vec_t(Beat) player2_beats;

// we create two separate beat arrays for each player
// so that they can play independently
void prepare_game_multiplayer(char* beatFile, Beatmap* currentBeatmap) {
  int boardLeft = 0;
  int boardRight = screenWidth / 2 + 4;
  int lineLength = 230;  // (screenWidth / 2 - 4) / 4

  if (beatFile == NULL || currentBeatmap == NULL) {
    printf("No beatmap selected!\n");
    setCurrentPage(PAGE_MENU);
    return;
  }

  vec_init(&player1_beats);
  vec_init(&player2_beats);

  int i;
  Beat* beat;
  vec_foreach_ptr(&currentBeatmap->beats, beat, i) {
    // beat->posX = boardLeft + lineLength * (beat->arrow + 0.5);
    // beat->posY = 100 - (beat->time * beat_speed / 15);  // adjust speed for multiplayer
    // vec_push(&player1_beats, *beat);
    // Beat beat2 = *beat;
    // beat2.posX = boardRight + lineLength * (beat->arrow + 0.5);
    // vec_push(&player2_beats, beat2);
    Beat beat1 = *beat;
    Beat beat2 = *beat;
    beat1.posX = boardLeft + lineLength * (beat->arrow + 0.5);
    beat2.posX = boardRight + lineLength * (beat->arrow + 0.5);
    int y;
    y = (screenHeight - 50) - (beat1.time * 60 * beat_speed);
    beat1.posY = y;
    beat2.posY = y;
    vec_push(&player1_beats, beat1);
    vec_push(&player2_beats, beat2);
  };

  _resetCounters();
  printf("loaded beats %d %d\n", player1_beats.length, player2_beats.length);
  _music = LoadMusicStream(currentBeatmap->music);
  _music.looping = false;

  int j;
  Beat* b;
  vec_foreach_ptr(&player1_beats, b, j) {
    // move the beat down
    printf("beat %d pos %d %d\n", j, b->posX, b->posY);
  }

  PlayMusicStream(_music);
}

void drawLeftPlayer();
void drawRightPlayer();

void drawMultiplayerPage(Beatmap* beatmap) {
  UpdateMusicStream(_music);

  if (_currentMPPage == PAUSE) {
    char* text = "Game Paused. Press P to resume.";
    DrawText(text, (screenWidth - MeasureText(text, 20)) / 2, screenHeight / 2, 20, WHITE);
    if(IsKeyPressed(KEY_Q)){
      _currentMPPage=END;
      return;
    }
    return;
  }

  if (_currentMPPage == END) {
    char* text = "Song ended! Press ENTER to return to menu.";
    DrawText(text, (screenWidth - MeasureText(text, textFontSize)) / 2, screenHeight / 2, textFontSize, WHITE);

    if (_music_unloaded) {
      StopMusicStream(_music);
      UnloadMusicStream(_music);
      _music_unloaded = 0;
    }

    if (IsKeyPressed(KEY_ENTER)) {
      setCurrentPage(PAGE_MENU);
      _currentMPPage = GAME;  // reset for next game
      _resetCounters();
      vec_clear(&beatmap->beats);
    }
    return;
  }

  if (IsMusicStreamPlaying(_music) == false && _currentMPPage == GAME) {
    // music ended, go to end screen
    _currentMPPage = END;
    return;
  }

  if (IsKeyPressed(KEY_P)) {
    if (_currentMPPage == GAME) {
      _currentMPPage = PAUSE;
      PauseMusicStream(_music);
    } else if (_currentMPPage == PAUSE) {
      _currentMPPage = GAME;
      ResumeMusicStream(_music);
    }
    return;
  }

  // DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

  drawLeftPlayer();
  drawRightPlayer();
}

void drawLeftPlayer() {
  int start = 0;
  int end = screenWidth / 2 - 4;
  int segment = 230;

  // TODO: use arrow textures on right
  Texture2D w = getArrowTextureAlpha(BEAT_UP);
  Texture2D a = getArrowTextureAlpha(BEAT_LEFT);
  Texture2D s = getArrowTextureAlpha(BEAT_DOWN);
  Texture2D d = getArrowTextureAlpha(BEAT_RIGHT);

  {
    // brown background
    DrawRectangle(start, 100, end, screenHeight, _GUITER_BODY);

    // border line
    DrawRectangleLinesEx((Rectangle){start, 100, end, screenHeight}, 10, _GUITER_BORDER);
    // vertical separators
    for (int i = 0; i < 4; i++) {
      DrawLineEx((Vector2){start + segment * (i + 0.5), 100}, (Vector2){start + segment * (i + 0.5), screenHeight - 15 - w.height}, 5, WHITE);
    }

    // horizontal guiter lines
    int bodyHeight = screenHeight - 200 - 15;
    for (int i = 1; i <= 2; i++) {
      DrawLineEx((Vector2){start + 10, bodyHeight / 3 * i + 100}, (Vector2){start + end - 10, bodyHeight / 3 * i + 100}, 5, WHITE);
    }

    // draw a circular border around the bottom arrow textures
    for (int i = 0; i < 4; i++) {
      DrawRing((Vector2){start + segment * (i + 0.5), screenHeight - (w.height / 2) - 20}, 50, 50 + 5, 0, 360, 100, WHITE);
    }

    DrawTexture(w, start + segment * 0.5 - w.width / 2, screenHeight - w.height - 20, WHITE);
    DrawTexture(s, start + segment * 1.5 - w.width / 2, screenHeight - w.height - 20, WHITE);
    DrawTexture(a, start + segment * 2.5 - w.width / 2, screenHeight - w.height - 20, WHITE);
    DrawTexture(d, start + segment * 3.5 - w.width / 2, screenHeight - w.height - 20, WHITE);

    DrawLineEx((Vector2){start, screenHeight - 135}, (Vector2){end, screenHeight - 135}, 5, WHITE);
  }

  for (int i = 0; i < player1_beats.length; i++) {
    Beat* beat = &player1_beats.data[i];
    int y = beat->posY;
    if (y > screenHeight) continue;  // off screen
    beat->posY += beat_speed;
    if (y < 100) break;                  // no need to check further, beats
    if (y > (screenHeight - 50 - 20)) {  // reached the bottom line
      if (!beat->pressed) {              // but was not pressed
        printf("Miss! Streak broken. beat time %d y %d\n", beat->time, y);
        streakL = 0;  // reset streak
      }
      continue;
    }
    // Texture2D texture = getArrowTextureAlpha(beat->arrow);
    // DrawTexture(texture, beat->posX - texture.width / 2, y - texture.height / 2, WHITE);
    drawBeat(beat, 1);

    int gap = (screenHeight - 50 - 20) - y;
    if (gap <= 100) {
      int keyAlpha = getKeyAlpha(beat->arrow);
      if (IsKeyPressed(keyAlpha) && !beat->pressed) {
        // hit
        beat->pressed = 1;
        if (gap <= 60) {
          streakL++;
          if (streakL > 99) streakL = 99;
          scoreL += streakL == 0 ? 300 : 300 * streakL;
          printf("Perfect! Streak %d Score %d\n", streakL, scoreL);
        } else {
          scoreL += streakL == 0 ? 100 : 100 * streakL;
        }
      }
    }
  }

  // draw stat section
  {
    DrawRectangle(start, 0, end, 100, BLACK);

    // streak counter
    int streak_10 = streakL / 10;
    int streak_1 = streakL % 10;

    DrawRectangleLinesEx((Rectangle){10 + 20 + 60 * 0, 20, 60, 60}, 5, WHITE);
    DrawRectangleLinesEx((Rectangle){10 + 20 + 60 * 1, 20, 60, 60}, 5, WHITE);
    DrawRectangleLinesEx((Rectangle){10 + 20 + 60 * 2, 20, 60, 60}, 5, WHITE);

    DrawText("x", 10 + 20 + 20, 30, 50, WHITE);
    DrawText(TextFormat("%d", streak_10), 10 + 20 + 60 * 1 + 20, 30, 50, WHITE);
    DrawText(TextFormat("%d", streak_1), 10 + 20 + 60 * 2 + 20, 30, 50, WHITE);

    // score counter
    GuiButton((Rectangle){screenWidth / 2 - 400, 20, 320, 80}, TextFormat("Score: %d", scoreL));
  }
}

void drawRightPlayer() {
  int start = screenWidth / 2 + 10;
  int end = screenWidth;
  int segment = 230;

  // TODO: use arrow textures on right
  Texture2D w = getArrowTexture(BEAT_UP);
  Texture2D a = getArrowTexture(BEAT_LEFT);
  Texture2D s = getArrowTexture(BEAT_DOWN);
  Texture2D d = getArrowTexture(BEAT_RIGHT);

  {
    // brown background
    DrawRectangle(start, 100, end, screenHeight, _GUITER_BODY);

    // border line
    DrawRectangleLinesEx((Rectangle){start, 100, end, screenHeight}, 10, _GUITER_BORDER);
    // vertical separators
    for (int i = 0; i < 4; i++) {
      DrawLineEx((Vector2){start + segment * (i + 0.5), 100}, (Vector2){start + segment * (i + 0.5), screenHeight - 15 - w.height}, 5, WHITE);
    }

    // horizontal guiter lines
    int bodyHeight = screenHeight - 200 - 15;
    for (int i = 1; i <= 2; i++) {
      DrawLineEx((Vector2){start + 10, bodyHeight / 3 * i + 100}, (Vector2){start + end - 10, bodyHeight / 3 * i + 100}, 5, WHITE);
    }

    // draw a circular border around the bottom arrow textures
    for (int i = 0; i < 4; i++) {
      DrawRing((Vector2){start + segment * (i + 0.5), screenHeight - (w.height / 2) - 20}, 50, 50 + 5, 0, 360, 100, WHITE);
    }

    DrawTexture(w, start + segment * 0.5 - w.width / 2, screenHeight - w.height - 20, WHITE);
    DrawTexture(s, start + segment * 1.5 - w.width / 2, screenHeight - w.height - 20, WHITE);
    DrawTexture(a, start + segment * 2.5 - w.width / 2, screenHeight - w.height - 20, WHITE);
    DrawTexture(d, start + segment * 3.5 - w.width / 2, screenHeight - w.height - 20, WHITE);

    DrawLineEx((Vector2){start, screenHeight - 135}, (Vector2){end, screenHeight - 135}, 5, WHITE);
  }

  for (int i = 0; i < player2_beats.length; i++) {
    Beat* beat = &player2_beats.data[i];
    int y = beat->posY;
    if (y > screenHeight) continue;  // off screen
    beat->posY += beat_speed;
    if (y < 100) break;                  // no need to check further, beats
    if (y > (screenHeight - 50 - 20)) {  // reached the bottom line
      if (!beat->pressed) {              // but was not pressed
        printf("Miss! Streak broken. beat time %d y %d\n", beat->time, y);
        streakR = 0;  // reset streak
      }
      continue;
    }
    // Texture2D texture = getArrowTextureAlpha(beat->arrow);
    // DrawTexture(texture, beat->posX - texture.width / 2, y - texture.height / 2, WHITE);
    drawBeat(beat, 0);

    int gap = (screenHeight - 50 - 20) - y;
    if (gap <= 100) {
      int keyAlpha = getKey(beat->arrow);
      if (IsKeyPressed(keyAlpha) && !beat->pressed) {
        // hit
        beat->pressed = 1;
        if (gap <= 60) {
          streakR++;
          if (streakR > 99) streakR = 99;
          scoreR += streakR == 0 ? 300 : 300 * streakR;
          printf("Perfect! Streak %d Score %d\n", streakR, scoreR);
        } else {
          scoreR += streakR == 0 ? 100 : 100 * streakR;
        }
      }
    }
  }

  // draw stat section
  {
    DrawRectangle(start, 0, end, 100, BLACK);

    // streak counter
    int streak_10 = streakR / 10;
    int streak_1 = streakR % 10;

    DrawRectangleLinesEx((Rectangle){start + 10 + 20 + 60 * 0, 20, 60, 60}, 5, WHITE);
    DrawRectangleLinesEx((Rectangle){start + 10 + 20 + 60 * 1, 20, 60, 60}, 5, WHITE);
    DrawRectangleLinesEx((Rectangle){start + 10 + 20 + 60 * 2, 20, 60, 60}, 5, WHITE);

    DrawText("x", start + 10 + 20 + 20, 30, 50, WHITE);
    DrawText(TextFormat("%d", streak_10), start + 10 + 20 + 60 * 1 + 20, 30, 50, WHITE);
    DrawText(TextFormat("%d", streak_1), start + 10 + 20 + 60 * 2 + 20, 30, 50, WHITE);

    // score counter
    GuiButton((Rectangle){screenWidth - 400, 20, 320, 80}, TextFormat("Score: %d", scoreR));
  }
}