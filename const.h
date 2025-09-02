#ifndef CONST_H
#define CONST_H

typedef enum {
    STARTING,
    MENU,
    SINGLEPLAYER,
    MULTIPLAYER,
    GENERATOR,
} Pages;

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;
extern const int FPS;

extern const char TITLE[];

#endif // CONST_H