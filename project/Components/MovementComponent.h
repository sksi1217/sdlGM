#pragma once

#include <SDL.h>
#include "../Core/Component.h"

class MovementComponent : public Component {
public:
    float Speed = 0.0f;                 // Скорость
    float SlowFactor = 1.0f;            // Фактор замедления
};