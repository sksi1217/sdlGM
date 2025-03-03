﻿#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "../Core/Component.h"
#include "../Resources/ManagerGame.h"

class Weapon : public GameObject {
public:
    // Конструктор
    Weapon(const SDL_FPoint& startPosition, SDL_Texture* texture);

    // Обновление состояния оружия
    void Update(float deltaTime) override;

private:
    GameObject* FindNearestEnemy();

    // Метод для расчета урона
    float CalculateDamage();

    // Проверка критического удара
    bool IsCriticalHit() {
        auto weapon = GetComponent<WeaponComponent>();
        return MathUtils::NextFloat() < weapon->m_criticalChance;
    }

protected:
    


    GameObject* m_nearestEnemy = nullptr; // Ближайший враг

    // Выстрел
    virtual void Shoot(const SDL_FPoint& Position);

    // Создание пули
    virtual void CreateBullet(const SDL_FPoint& position, const SDL_FPoint& direction, SDL_Texture* texture);
};

#endif // WEAPON_H