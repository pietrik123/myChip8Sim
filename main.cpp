#include "SDL.h"
#include "MyGfx.h"
#include "Chip.h"

#include <iostream>
#include <queue>
#include <stack>

bool shouldQuit();

int main(int argc, char** argv)
{
    std::cout << "Hello world!\n"; 

    MyGfx gfx(64,32,8);
    MyChip8::Chip myChip {};
    
    SDL_Delay(500);
    
    
    myChip.logMemory(80,80);
    myChip.logRegisters();
    
    myChip.loadProgram("build\\ibm.ch8");
    
    int cnt = 0;
    while (true)
    {
        uint16_t rawCmd = myChip.fetch();
        auto instrData = myChip.decode(rawCmd);
        myChip.execute(&gfx, instrData);
        
        cnt++;
        
        if (shouldQuit())
        {
            break;
        }
        
        SDL_Delay(50);
    }
    
    SDL_Delay(200);
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

