#pragma once

#include "SDL2/SDL.h"

#include <optional>

bool isKeyPressed(uint8_t key);
std::optional<uint8_t> getPressedKey();
