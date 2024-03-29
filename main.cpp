#include "SDL2/SDL.h"
#include "MyGfx.h"
#include "Chip.h"

#include <iostream>
#include <queue>
#include <stack>

#include <chrono>

bool shouldQuit();

int main(int argc, char** argv)
{
    
    if (argc < 2)
    {
        std::cout << "Please provide a program file as argument!\n";
        return 1;
    }
    
    std::cout << "Hello world!\n"; 

    MyGfx gfx(64,32,8);
    MyChip8::Chip myChip {};
    
    SDL_Delay(500);
    
    myChip.logMemory(80,80);
    myChip.logRegisters();
    
    if (!myChip.loadProgram(argv[1]))
    {
        std::cout << "Failed to load the program!\n";
        return 1;
    }
    
    int cnt = 0;
    while (true)
    {
        auto t1 = std::chrono::system_clock::now();

        uint16_t rawCmd = myChip.fetch();
        auto instrData = myChip.decode(rawCmd);
        myChip.execute(&gfx, instrData);
        // myChip.logRegisters();
        myChip.handleDelayTimer();
        myChip.handleSoundTimer();
        
        cnt++;
        
        if (shouldQuit())
        {
            break;
        }

        //SDL_Delay(20);

        auto t2 = std::chrono::system_clock::now();

        std::cout << "Elapsed: " << std::dec << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << "\n";
        
        SDL_Delay(1);
    }
    
    SDL_Delay(200);
    return 0;
}

bool shouldQuit()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
    {
        return true;
    }
    return false;
}

