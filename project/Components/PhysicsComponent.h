#pragma once

#include <SDL.h>
#include "../Core/Component.h"

class PhysicsComponent : public Component {
public:
    SDL_FPoint Velocity = { 0, 0 };
    SDL_FPoint Acceleration = { 0, 0 };
    float Drag = 0.98f; // Коэффициент трения
    float Mass = 1.0f;
    float PushForce = 1.0f; // Сила отталкивания
};