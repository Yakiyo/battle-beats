#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define screenwidth 1280
#define screenheight 720

//screen texts

const char *title = "BATTLE BEATS";
const char *startText = "Press ENTER to Start";
const char *singleplayerText = "Endless Encore";
const char *multiplayerText = "Beat Duel";

// font sizes and text widths

int titleFontSize = 100;
int textWidth = 0;
int textFontSize = 50;
int startTextWidth = 0;

//define screen states

typedef enum {
    TITLE,
    MENU,
    SINGLEPLAYER,
    MULTIPLAYER
}screen;
screen currentScreen=TITLE;

int main(void)
{
    InitWindow(screenwidth, screenheight, "Battle Beats - Menu");
    SetTargetFPS(60);

    // Set GUI text properties

    GuiSetStyle(DEFAULT, TEXT_SIZE, 50);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 2);

    // Transparent backgrounds

    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x00000000);
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x00000000);
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x00000000);

    GuiSetStyle(DEFAULT, BORDER_WIDTH, 0);  // hide outline

    // Text colors

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);   // White
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0xFF69B4FF);  // pink hover
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, 0xFF69B4FF);  // pink press

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        // screen handling

        switch(currentScreen){

            // title screen

            case TITLE:{

                // center the title text
                if (textWidth == 0) textWidth = MeasureText(title, titleFontSize);
                int x = (screenwidth - textWidth) / 2;
                int y = 100;

                // draw title
                DrawText("BATTLE BEATS", x, y, titleFontSize, PINK);
                textWidth = 0; // reset for next frame

                // center the start text

                if (startTextWidth == 0) startTextWidth = MeasureText(startText, textFontSize);
                x = (screenwidth - startTextWidth) / 2;
                y = screenheight / 2;

                // draw start text

                DrawText(startText, x, y, textFontSize, RAYWHITE);
                startTextWidth = 0; // reset for next frame

                // changing current screen to menu

                if(IsKeyPressed(KEY_ENTER)) currentScreen=MENU;

                if(IsKeyPressed(KEY_ESCAPE))    break;

            }break;

            // menu screen

            case MENU:{

                // draw singleplayer and multiplayer options

                    // align singleplayer button in center
                if (textWidth == 0) textWidth = MeasureText(singleplayerText, textFontSize);
                int x=(screenwidth - textWidth) / 2;
                int y=screenheight / 2 - 50;

                if(GuiButton((Rectangle){x, y, textWidth + 20, 40}, singleplayerText)) {
                    // Start singleplayer mode
                    currentScreen=SINGLEPLAYER;

                }
                textWidth = 0; // reset for next frame
                    // align multiplayer button in center
                if(textWidth == 0) textWidth = MeasureText(multiplayerText, textFontSize);
                x=(screenwidth - textWidth) / 2;
                y=screenheight / 2 + 50;

                if(GuiButton((Rectangle){x, y, textWidth + 20, 40}, multiplayerText)) {
                    // Start multiplayer mode
                    currentScreen=MULTIPLAYER;
                    textWidth = 0;
                }
                textWidth = 0; // reset for next frame

                if(IsKeyPressed(KEY_ESCAPE))    break;

            }break;

            // singleplayer and multiplayer screens

            case SINGLEPLAYER:{

                // Start singleplayer game

                // DrawText("Singleplayer Mode - Press Q to return", 100, 100, 20, RAYWHITE);

                

                if(IsKeyPressed(KEY_Q)) currentScreen=MENU;
            }break;

            case MULTIPLAYER:{

                // Start multiplayer game

                DrawText("Multiplayer Mode - Press Q to return", 100, 100, 20, RAYWHITE);

                if(IsKeyPressed(KEY_Q)) currentScreen=MENU;
            }break;

        }
        // screen handling end
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


