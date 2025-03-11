#pragma once

#include "../Core/Component.h"
#include "../Core/Entities.h"

#include "../Resources/ManagerGame.h"
#include "../Core/Weapon.h"

class Player : public Entities {
public:
    // Конструктор с параметрами
    Player(const SDL_FPoint& startPosition, SDL_Texture* texture);

    void Update(float deltaTime) override;

    void Draw(SDL_Renderer* renderer, const Camera& camera) override;

protected:
    void ResourceGathering(SDL_FPoint& posistionPlr, float deltaTime);

    void InitializeComponent(const SDL_FPoint& startPosition, SDL_Texture* texture) override {
        Entities::InitializeComponent(startPosition, texture);

        auto level = std::make_shared<LevelComponent>();
        AddComponent(level);
    }
private:
    // float ElapsedTime = 0;

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
