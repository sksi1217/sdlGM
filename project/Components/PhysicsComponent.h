#pragma once

#include <SDL.h>
#include "../Core/Component.h"

class PhysicsComponent : public Component {
public:
    float PushForce = 1.0f;             // Сила отталкивания
    float Mass = 50.0f;                 // Масса
    SDL_FPoint velocity = { 0.0f, 0.0f };
};