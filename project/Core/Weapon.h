#pragma once
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

    // Выстрел
    virtual void Shoot(SDL_FPoint& Position);
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

    // Создание пули
    virtual void CreateBullet(SDL_FPoint& position, const SDL_FPoint& direction, SDL_Texture* texture);
};

#endif // WEAPON_H