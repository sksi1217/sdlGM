#pragma once

#include <SDL.h>
#include <iostream>
#include <algorithm>

#include "../Resources/ManagerGame.h"
#include "../Utils/TextureLoader.h"

#include "../Core/EntityHeaders.h"
#include "../Core/Experience.h"

class Experience;

class HealthComponent : public Component {
public:
    float m_currentHealth = 100.0f; // Текущее здоровье
    float m_maxHealth = 100.0f;     // Максимальное здоровье

    float m_vampirism = 0.0f;

    float m_regenRate = 1.0f;       // Скорость регенерации здоровья

    float m_invincibilityTime = 0.0f; // Время неуязвимости после получения урона
    float m_remainingInvincibility = 0.0f; // Оставшееся время неуязвимости
    bool IsDead = false;          // Флаг смерти

    // Нанести урон объекту
    void TakeDamage(float damage) {
        if (m_remainingInvincibility > 0.0f) return; // Неуязвим во время инвулNERA
        m_currentHealth = std::max(0.0f, m_currentHealth - damage);
        m_remainingInvincibility = m_invincibilityTime;

        if (m_currentHealth <= 0.0f) {
            IsDead = true; // Устанавливаем флаг смерти
        }
    }

    // Регенерация здоровья
    void RegenerateHealth(float deltaTime) {
        if (m_currentHealth < m_maxHealth) {
            m_currentHealth = std::min(m_maxHealth, m_currentHealth + m_regenRate * deltaTime);
        }
    }

    // Обновление состояния компонента
    void Update(float deltaTime) {
        if (m_remainingInvincibility > 0.0f) {
            m_remainingInvincibility -= deltaTime;
        }
        RegenerateHealth(deltaTime);
    }

    // Проверка, умер ли объект
    bool IsDeadState() const {
        return IsDead;
    }
};