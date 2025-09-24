#include <stdio.h>

#include "const.h"
#include "external/vec.h"
#include "windows.h"

vec_str_t beatmap_files;
vec_str_t music_files;

const char *_beatmap_dir = ".\\beatmaps";

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
    char *name = _strdup(findFileData.cFileName);  // Windows-safe strdup
    vec_push(&beatmap_files, name);
  } while (FindNextFile(hFind, &findFileData) != 0);
}

vec_str_t* getMusicFiles() {
  vec_init(&music_files);

  WIN32_FIND_DATA findFileData;
  HANDLE hFind = INVALID_HANDLE_VALUE;

  char searchPath[MAX_PATH];
  snprintf(searchPath, MAX_PATH, "%s\\*.mp3", _beatmap_dir);

  hFind = FindFirstFile(searchPath, &findFileData);

  if (hFind == INVALID_HANDLE_VALUE) {
    printf("No .mp3 files found in folder: %s\n", _beatmap_dir);
    return &music_files;
  }

  do {
    // Allocate and copy the filename
    char *name = _strdup(findFileData.cFileName);  // Windows-safe strdup
    vec_push(&music_files, name);
  } while (FindNextFile(hFind, &findFileData) != 0);

  return &music_files;
}

void unloadMusicFiles() {
  vec_deinit(&music_files);
}

void unloadBeatmaps() {
  vec_deinit(&beatmap_files);
}
