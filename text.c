#include<stdio.h>
#include "raylib.h"

void prnt() {
    printf("Hello, World!\n");
    if (IsWindowFocused()) {
        printf("The window is focused.\n");
    } else {
        printf("The window is not focused.\n");
    }
}