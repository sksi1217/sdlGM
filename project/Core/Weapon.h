#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "../Core/Component.h"
#include "../Resources/ManagerGame.h"
#include "../Entities/Player.h"

class Player;

class Weapon : public GameObject {
public:
    // Конструктор
    Weapon(const SDL_FPoint& startPosition, SDL_Texture* texture);

    // Выстрел
    virtual void Shoot(Player* player);
private:
    GameObject* FindNearestEnemy(Player* player);

    // Метод для расчета урона
    float CalculateDamage();

    // Проверка критического удара
    bool IsCriticalHit() {
        auto weapon = GetComponent<WeaponComponent>();
        return MathUtils::NextFloat() < weapon->m_criticalChance;
    }

protected:
    GameObject* m_nearestEnemy = nullptr; // Ближайший враг

    // Создание пули
    virtual void CreateBullet(SDL_FPoint& position, const SDL_FPoint& direction, SDL_Texture* texture);
};

#endif // WEAPON_H