set SDL2_DEV_DIR=C:\Users\pietr\libraries\SDL2-devel-2.26.2-mingw\SDL2-2.26.2\x86_64-w64-mingw32\
g++ -g^
 main.cpp MyGfx.cpp MyKeyboardInput.cpp Chip.cpp MyChipLogging.cpp^
 MyChip8Instructions.cpp MyChip8InstructionsGfxInput.cpp^
 -I%SDL2_DEV_DIR%include\ -L%SDL2_DEV_DIR%\lib\ -lmingw32 -lSDL2main -lSDL2^
 -o build\chip8.exe