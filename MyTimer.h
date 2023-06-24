#pragma once

#include "SDL2/SDL.h"

class MyTimerWrapper
{
private:
    bool m_started = false;
    SDL_TimerID m_id;
    unsigned int m_delayMs;
public:
    MyTimerWrapper(unsigned int delayMs);

    void start(unsigned int (*func)(unsigned int, void*), void* data);

    ~MyTimerWrapper();
};