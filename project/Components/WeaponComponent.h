﻿#pragma once

#include "../Utils/MathUtils.h"
#include "../Core/Component.h"
#include <SDL.h>

class WeaponComponent : public Component {
public:
    // Боевые характеристики

    // Все что связано с пулей
    int m_damage = 10; // Базовый урон
    float m_criticalChance = 0.1f; // Шанс критического удара (0 = 0%, 1 = 100%)
    float m_criticalMultiplier = 2.0f; // Множитель критического удара
    float m_lifetimeBullet = 1.0f; // Время жизни пули
    SDL_FPoint m_direction = { 0, 0 }; // Направление пули
    
    // Вампиризм
    // Все что связано с оружием
    float m_range = 100.0f; // Дальность атаки
    int m_shells = 1; // Кол-во снарядов
    float m_fireRate = 5; // Интервал между залпами (в секундах)
    float m_bulletInterval = 0.5; // Интервал между пулями внутри залпа (в секундах)
    bool im_isFiringZalp = false; // Флаг, указывающий, происходит ли залп
    float accuracy = 1; // точность

    // Другое
    int m_shellsToFire = 0; // Оставшееся количество пуль в текущем залпе
    float m_lastShotTime = 0.0f; // Время последнего выстрела
    float m_elapsedTime = 0.0f; // Время, прошедшее с последнего выстрела
};