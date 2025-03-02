#pragma once

#include "../Core/GameObject.h"
#include "../Components/TransformComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/StateComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/RenderComponent.h"
#include "../Utils/TextureLoader.h"

class Skelet : public GameObject {
public:
    // Конструктор с параметрами
    Skelet(const SDL_FPoint& startPosition, SDL_Texture* texture);

    void Update(float deltaTime) override;

private:
    SDL_FPoint direction;
    SDL_FPoint targetPosition = { 100, 100 };

    void HandleMovement(float deltaTime);

    enum Direction {
        DownRow = 1,
        UpRow = 2,
        RightRow = 3,
        LeftRow = 4,
    };
};
