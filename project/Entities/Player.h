#pragma once

#include "../Core/Component.h"

#include "../Resources/ManagerGame.h"
#include "../Core/Weapon.h"

class Player : public GameObject {
public:
    // Конструктор с параметрами
    Player(const SDL_FPoint& startPosition, SDL_Texture* texture);

    void Update(float deltaTime) override;

    void Draw(SDL_Renderer* renderer, const Camera& camera) override;

protected:
    void ResourceGathering(SDL_FPoint& posistionPlr);

private:
    float ElapsedTime = 0;

    void HandleWeaponInteraction(float deltaTime);
    void HandleMovement(const Uint8* keyboardState, SDL_FPoint& velocity, float deltaTime);
    void UpdateSpriteRow(const SDL_FPoint& velocity);

    enum Direction {
        DownRow = 1,
        UpRow = 2,
        RightRow = 3,
        LeftRow = 4,
    };
};
