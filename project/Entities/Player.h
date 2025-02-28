#pragma once

#include "../Core/GameObject.h"
#include "../Components/TransformComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/StateComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/RenderComponent.h"

class Player : public GameObject {
public:
    Player() {
        AddComponent(std::make_shared<TransformComponent>());
        AddComponent(std::make_shared<MovementComponent>());
        AddComponent(std::make_shared<StateComponent>());
        AddComponent(std::make_shared<PhysicsComponent>());
        AddComponent(std::make_shared<RenderComponent>());
        AddComponent(std::make_shared<AnimationComponent>());
    }

    void Update(float deltaTime);
};
