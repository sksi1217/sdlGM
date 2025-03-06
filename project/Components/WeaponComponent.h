#pragma once

#include "../Utils/MathUtils.h"
#include "../Core/Component.h"
#include <SDL.h>

class WeaponComponent : public Component {
public:
    // Метод для расчета урона
    float CalculateDamage() {
        float totalDamage = m_damage;
        if (IsCriticalHit()) {
            totalDamage *= m_criticalMultiplier;
        }
        return totalDamage;
    }

    // Проверка критического удара
    bool IsCriticalHit() {
        return MathUtils::NextFloat() < m_criticalChance;
    }


    bool m_remove_bullet = true;

    // Боевые характеристики
    int m_damage = 10; // Базовый урон
    float m_criticalChance = 0.1f; // Шанс критического удара (0 = 0%, 1 = 100%)
    float m_criticalMultiplier = 2.0f; // Множитель критического удара


    float m_range = 50.0f; // Дальность атаки

    // Вампиризм

    int m_shells = 1; // Кол-во снарядов
    float m_fireRate = 1.0f; // Интервал между залпами (в секундах)
    float m_bulletInterval = 1.0f; // Интервал между пулями внутри залпа (в секундах)
    bool im_isFiringZalp = false; // Флаг, указывающий, происходит ли залп
    float m_lifetimeBullet = 1.0f; // Время жизни пули

    SDL_FPoint m_direction = { 0, 0 }; // Направление пули

    int m_shellsToFire = 0; // Оставшееся количество пуль в текущем залпе
    float m_lastShotTime = 0.0f; // Время последнего выстрела
    float m_elapsedTime = 0.0f; // Время, прошедшее с последнего выстрела
};