#include "MyTimer.h"

MyTimerWrapper::MyTimerWrapper(unsigned int delayMs) : m_delayMs(delayMs)
{
}

void MyTimerWrapper::start(unsigned int (*func)(unsigned int, void*), void* data)
{
    if (m_started)
    {
        return;
    }
    m_id = SDL_AddTimer(m_delayMs, func, data);
    m_started = true;
}


MyTimerWrapper::~MyTimerWrapper()
{
    SDL_RemoveTimer(m_id);
}