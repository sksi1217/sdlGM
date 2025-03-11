#pragma once

#include "../Components/EntitiesComponent.h"
#include "../Core/Entities.h"
#include "../Components/Events/EnemyDeathComponent.h"

class Skelet : public Entities {
public:
    // Конструктор с параметрами
    Skelet(const SDL_FPoint& startPosition, SDL_Texture* texture, std::shared_ptr<TransformComponent> playerTransform);

    void Update(float deltaTime) override;

private:
    SDL_FPoint direction = { 0, 0 };
    SDL_FPoint targetPosition = { 0, 0 };

    void HandleMovement(float deltaTime);

    std::shared_ptr<TransformComponent> playerTransform;

    enum Direction {
        DownRow = 1,
        UpRow = 2,
        RightRow = 3,
        LeftRow = 4,
    };
};
