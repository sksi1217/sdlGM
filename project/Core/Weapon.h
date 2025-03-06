#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "../Core/EntityHeaders.h"
#include "../Resources/ManagerGame.h"
#include "../Entities/Player.h"
#include "../Core/GameObject.h"

class Player;

class Weapon : public GameObject {
public:
    // Конструктор
    Weapon(SDL_Texture* texture);

    // Выстрел
    virtual void Shoot(Player* player, float deltaTime);

protected:
    GameObject* m_nearestEnemy = nullptr; // Ближайший враг

    // Создание пули
    virtual void CreateBullet(SDL_FPoint& position, const SDL_FPoint& direction, SDL_Texture* texture);

private:
    GameObject* FindNearestEnemy(Player* player);
};

#endif // WEAPON_H