#ifndef CONST_FILE_H
#define CONST_FILE_H

extern const int screenWidth;
extern const int screenHeight;
extern const int screenWUnit;
extern const int headerHeight;
extern const int beatRadius;
extern const int beatSpeed;

typedef enum PageType {
    PAGE_OPENING,
    PAGE_MENU,
    PAGE_SINGLEPLAYER,
    PAGE_MULTIPLAYER,
    PAGE_GENERATOR,
    QUIT_GAME
} PageType;


#endif // CONST_FILE_H