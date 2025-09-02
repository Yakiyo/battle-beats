gcc -o main.exe main.c const.c -I include -L lib -lraylib -lgdi32 -lwinmm; if ($?) { .\main.exe }
