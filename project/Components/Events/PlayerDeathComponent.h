#pragma once
#include "../DeathComponent.h"

class PlayerDeathComponent : public DeathComponent {
public:
    void OnDeath(GameObject* target) override {
        // Базовая логика (создание опыта и деактивация)
        DeathComponent::OnDeath(target); // Вызываем базовый метод
    }
};