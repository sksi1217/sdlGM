#pragma once

#include "../Core/Component.h"
#include <SDL.h>

class WeaponComponent : public Component {
public:
    int m_damage = 10; // Базовый урон
    float m_criticalChance = 0.05f; // Шанс критического удара
    float m_criticalMultiplier = 2.0f; // Множитель критического удара
    float m_range = 200.0f; // Дальность атаки
    int m_shells = 1; // Кол-во снарядов
    float m_fireRate = 2.0f; // Интервал между залпами (в секундах)
    float m_bulletInterval = 0.4f; // Интервал между пулями внутри залпа (в секундах)
    int m_shellsToFire = 0; // Оставшееся количество пуль в текущем залпе
    bool im_isFiringZalp = false; // Флаг, указывающий, происходит ли залп

    float m_lifetimeBullet = 5.0f; // Время жизни пули
    float m_lastShotTime = 0.0f; // Время последнего выстрела
    float m_elapsedTime = 0.0f; // Время, прошедшее с последнего выстрела
};