#pragma once

#include <SDL.h>
#include "../Core/Component.h"

class StateComponent : public Component {
public:
    bool IsActive = true;               // Активность
    bool IsCollidable = true;           // Возможность коллизий
    bool IsStatic = false;              // Статичность
};