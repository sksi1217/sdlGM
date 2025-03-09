#pragma once

#include "../Utils/MathUtils.h"
#include "../Core/Component.h"
#include <random>
#include <SDL.h>

class WeaponComponent : public Component {
public:
    static std::mt19937& GetRandomGenerator() {
        static std::mt19937 gen(std::random_device{}());
        return gen;
    }

    float GetEffectiveFireRate(float cooldown) const {
        return m_fireRate - cooldown ? cooldown : 0;
    }

    void StartBurst() {
        im_isFiringZalp = true;
        m_shellsToFire = m_shells; // Количество пуль в залпе
        m_elapsedTime = 0; // Сбрасываем таймер
    }

    void EndBurst() {
        im_isFiringZalp = false;
        m_elapsedTime = 0;
    }

    // Все что связано с пулей
    int m_damage = 10; // Базовый урон (оружия)
    float m_criticalChance = 0.1f; // Шанс критического удара (0 = 0%, 1 = 100%) (оружия)
    // Перезарядка
    float m_fireRate = 5; // Интервал между залпами (в секундах)
    float m_bulletInterval = 0.5; // Интервал между пулями внутри залпа (в секундах)
    float m_range = 100.0f; // Дальность атаки (оружия)
    int m_shells = 1; // Кол-во снарядов
    float accuracy = 0.5; // точность

    float m_lifetimeBullet = 1;

    // Менять не нужно (Другое)
    bool im_isFiringZalp = false; // Флаг, указывающий, происходит ли залп
    SDL_FPoint m_direction = { 0, 0 }; // Направление пули
    int m_shellsToFire = 0; // Оставшееся количество пуль в текущем залпе
    float m_lastShotTime = 0.0f; // Время последнего выстрела
    float m_elapsedTime = 0.0f; // Время, прошедшее с последнего выстрела
};