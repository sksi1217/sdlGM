#pragma once

#include "../Core/GameObject.h"
#include "../Components/TransformComponent.h"

class Player : public GameObject {
public:
    Player() {
        AddComponent(std::make_shared<TransformComponent>());
    }
};
