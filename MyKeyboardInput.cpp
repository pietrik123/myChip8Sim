#include "MyKeyboardInput.h"

#include <map>

static const std::map<uint8_t, SDL_Scancode> keyBindings
{
    { 0x00, SDL_SCANCODE_X },
    { 0x01, SDL_SCANCODE_1 },
    { 0x02, SDL_SCANCODE_2 },
    { 0x03, SDL_SCANCODE_3 },
    { 0x04, SDL_SCANCODE_Q },
    { 0x05, SDL_SCANCODE_W },
    { 0x06, SDL_SCANCODE_E },
    { 0x07, SDL_SCANCODE_A },
    { 0x08, SDL_SCANCODE_S },
    { 0x09, SDL_SCANCODE_D },
    { 0x0a, SDL_SCANCODE_Z },
    { 0x0b, SDL_SCANCODE_C },
    { 0x0c, SDL_SCANCODE_4 },
    { 0x0d, SDL_SCANCODE_R },
    { 0x0e, SDL_SCANCODE_F },
    { 0x0f, SDL_SCANCODE_V }
};

bool isKeyPressed(uint8_t key)
{
    const auto keyboardState = SDL_GetKeyboardState(nullptr);
    const auto code = keyBindings.at(key);

    return keyboardState[code];
}

std::optional<uint8_t> getPressedKey()
{
    for (uint8_t i = 0u; i < 16u; i++)
    {
        if (isKeyPressed(i))
        {
            return {i};
        }
    }
    return std::nullopt;
}

