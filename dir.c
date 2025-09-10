#include <stdio.h>

#include "external/vec.h"
#include "windows.h"
#include "const.h"

vec_str_t beatmap_files;

const char* _beatmap_dir = ".\\beatmaps";

void loadBeatmaps() {
    vec_init(&beatmap_files);

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char searchPath[MAX_PATH];
    snprintf(searchPath, MAX_PATH, "%s\\*.txt", _beatmap_dir);

     hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No .txt files found in folder: %s\n", _beatmap_dir);
        return;
    }

    do {
        // Allocate and copy the filename
        char *name = _strdup(findFileData.cFileName); // Windows-safe strdup
        vec_push(&beatmap_files, name);
    } while (FindNextFile(hFind, &findFileData) != 0);
}

void unloadBeatmaps() {
    vec_deinit(&beatmap_files);
}

