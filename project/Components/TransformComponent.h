#pragma once
#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <SDL.h>
#include "../Core/GameObject.h"

class TransformComponent : public Component {
public:
    SDL_FPoint Position = { 10.0f, 0.0f };
    float Speed = 50.0f;
    SDL_Rect Collider = { 0, 0, 0, 0 };
};

#endif // TRANSFORM_COMPONENT_H