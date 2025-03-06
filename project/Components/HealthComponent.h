#pragma once

#include <SDL.h>
#include <algorithm>
#include "../Core/Component.h"

class HealthComponent : public Component {
public:
    float CurrentHealth = 100.0f; // Текущее здоровье
    float MaxHealth = 100.0f;     // Максимальное здоровье
    float RegenRate = 1.0f;       // Скорость регенерации здоровья (например, +1.0 за секунду)
    float InvincibilityTime = 0.0f; // Время неуязвимости после получения урона (в секундах)
    float RemainingInvincibility = 0.0f; // Оставшееся время неуязвимости
    bool IsDead = false;          // Флаг смерти

    // Нанести урон объекту
    void TakeDamage(float damage) {
        if (RemainingInvincibility > 0.0f) return; // Неуязвим во время инвулNERA
        CurrentHealth = std::max(0.0f, CurrentHealth - damage);
        RemainingInvincibility = InvincibilityTime;
        if (CurrentHealth <= 0.0f) {
            IsDead = true;
        }
    }

    // Регенерация здоровья
    void RegenerateHealth(float deltaTime) {
        if (CurrentHealth < MaxHealth) {
            CurrentHealth = std::min(MaxHealth, CurrentHealth + RegenRate * deltaTime);
        }
    }

    // Обновление состояния компонента (например, для обработки неуязвимости)
    void Update(float deltaTime) {
        if (RemainingInvincibility > 0.0f) {
            RemainingInvincibility -= deltaTime;
        }
        RegenerateHealth(deltaTime);
    }
};