#pragma once

#include "../Core/Component.h"

#include "../Utils/TextureLoader.h"
#include "../Resources/ManagerGame.h"
#include "../Core/Weapon.h"

class Player : public GameObject {
public:
    // Конструктор с параметрами
    Player(const SDL_FPoint& startPosition, SDL_Texture* texture);

    void Update(float deltaTime) override;

private:
    

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
