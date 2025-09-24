#include <math.h>

#include "assets.h"
#include "const.h"
#include "raygui.h"
#include "raylib.h"

PageType _currentPage = PAGE_OPENING;

PageType getCurrentPage() { return _currentPage; }

void setCurrentPage(PageType page) { _currentPage = page; }

float _fader_timer = 0.0f;

void drawOpeningPage(int is_menu) {
  char* title = "Battle Beats";
  int titleWidth = MeasureText(title, titleFontSize);

  DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 4,
           titleFontSize, PINK);

  if (!is_menu) {
    // change alpha over time which makes the text fade in and out
    _fader_timer += GetFrameTime();
    float alpha =
        (sinf(_fader_timer * 2.0f) + 1.0f) * 0.5f * 255;  // 2.0f is speed

    char* text = "Press ENTER to continue";
    DrawText(text, (screenWidth - MeasureText(text, textFontSize)) / 2,
             // light gray color
             screenHeight / 2, textFontSize, (Color){200, 200, 200, alpha});

    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      setCurrentPage(PAGE_MENU);
      _fader_timer = 0.0f;  // reset timer for next use
    }
  } else {
    int buttonWidth = 500;
    int buttonHeight = 100;
    int b_singleplayer =
        GuiButton((Rectangle){(screenWidth - buttonWidth) / 2,
                              screenHeight / 2 - 60, buttonWidth, buttonHeight},
                  "Singleplayer");

    int b_multiplayer = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2,
                    screenHeight / 2 - 60 + 100, buttonWidth, buttonHeight},
        "Multiplayer");
    int b_generator = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2,
                    screenHeight / 2 - 60 + 200, buttonWidth, buttonHeight},
        "Beatmap Generator");

    int b_quit = GuiButton(
        (Rectangle){(screenWidth - buttonWidth) / 2,
                    screenHeight / 2 - 60 + 300, buttonWidth, buttonHeight},
        "Quit Game");

    DrawTexture(charTexture1, screenWidth / 5 - charTexture1.width / 2,
                screenHeight / 2 - charTexture1.height / 2, WHITE);

    if (b_singleplayer) {
      setCurrentPage(BEAT_SELECTION_SINGLE);
    } else if (b_multiplayer) {
      setCurrentPage(BEAT_SELECTION_MULTI);
    } else if (b_generator) {
      setCurrentPage(BEAT_SELECTION_GENERATOR);
    } else if (b_quit) {
      setCurrentPage(QUIT_GAME);
    }
  }
}