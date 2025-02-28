#pragma once

#include "../Core/GameObject.h"
#include "../Components/TransformComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/StateComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/RenderComponent.h"
#include "../Utils/TextureLoader.h"
#include "../Resources/ManagerGame.h"

class Player : public GameObject {
public:
    // Конструктор с параметрами
    Player(const SDL_FPoint& startPosition, SDL_Texture* texture);

    void Update(float deltaTime) override;

private:
    void HandleMovement(const Uint8* keyboardState, SDL_FPoint& velocity, float deltaTime);
    void UpdateSpriteRow(const SDL_FPoint& velocity);

    enum Direction {
        DownRow = 1,
        UpRow = 2,
        RightRow = 3,
        LeftRow = 4,
    };
};
