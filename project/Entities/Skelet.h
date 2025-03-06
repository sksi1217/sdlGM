#pragma once

#include "../Core/EntityHeaders.h"

class Skelet : public GameObject {
public:
    // Конструктор с параметрами
    Skelet(const SDL_FPoint& startPosition, SDL_Texture* texture);

    void Update(float deltaTime) override;

private:
    SDL_FPoint direction ;
    SDL_FPoint targetPosition = { 1000, 1000 };

    void HandleMovement(float deltaTime);

    enum Direction {
        DownRow = 1,
        UpRow = 2,
        RightRow = 3,
        LeftRow = 4,
    };
};
