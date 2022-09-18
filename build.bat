set SDL2_DEV_DIR=C:\development\sdl2_code_blocks\
g++ -g main.cpp MyGfx.cpp Chip.cpp -std=c++11 -I%SDL2_DEV_DIR%include\ -L%SDL2_DEV_DIR%\lib\ -lmingw32 -lSDL2main -lSDL2 -o build\chip8.exe