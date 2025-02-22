@echo off
set PATH=%PATH%;C:\raylib\w64devkit\bin
g++ -O1 src/*.cpp src_dependecies/*.c -o bin/world.exe -Wall -Wno-missing-braces -I include/. -I header/. -L lib -lSDL2 -lSDL2main -lopengl32 -lgdi32 -llua54 -lenet64 -lwinmm -lws2_32 -luser32 -lshell32
bin\world.exeg++ -O1 src/*.cpp src_dependecies/*.c -o bin/world.exe -Wall -Wno-missing-braces -I include/. -I header/. -L lib -lSDL2 -lSDL2main -lopengl32 -lgdi32 -llua54 -lenet64 -lwinmm -lws2_32 -luser32 -lshell32
bin\world.exe