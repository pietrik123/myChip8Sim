REM set INCLDIR=C:\development\sdl2_visual\sdl2_ttf\include;C:\development\sdl2_visual\sdl2_mixer\include;C:\development\sdl2_visual\sdl2_image\include;C:\development\sdl2_visual\include
REM set LIBDIR=C:\development\sdl2_visual\sdl2_ttf\lib\x86;c:\development\sdl2_visual\sdl2_mixer\lib\x86;C:\development\sdl2_visual\sdl2_image\lib\x86;C:\development\sdl2_visual\lib\x86
REM SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_mixer.lib;SDL2_ttf.lib

REM , -w -Wl -subsystem,windows

g++ -g main.cpp MyGfx.cpp Chip.cpp -std=c++11 -IC:\development\sdl2_code_blocks\include\SDL2 -LC:\development\sdl2_code_blocks\lib\  -lmingw32 -lSDL2main    -lSDL2 -o build/chip8.exe