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
    float CurrentHealth = 100.0f; // Текущее здоровье
    float MaxHealth = 100.0f;     // Максимальное здоровье
    float RegenRate = 1.0f;       // Скорость регенерации здоровья
    float InvincibilityTime = 0.0f; // Время неуязвимости после получения урона
    float RemainingInvincibility = 0.0f; // Оставшееся время неуязвимости
    bool IsDead = false;          // Флаг смерти

    // Нанести урон объекту
    void TakeDamage(float damage) {
        if (RemainingInvincibility > 0.0f) return; // Неуязвим во время инвулNERA
        CurrentHealth = std::max(0.0f, CurrentHealth - damage);
        RemainingInvincibility = InvincibilityTime;

        if (CurrentHealth <= 0.0f) {
            IsDead = true; // Устанавливаем флаг смерти
        }
    }

    // Регенерация здоровья
    void RegenerateHealth(float deltaTime) {
        if (CurrentHealth < MaxHealth) {
            CurrentHealth = std::min(MaxHealth, CurrentHealth + RegenRate * deltaTime);
        }
    }

    // Обновление состояния компонента
    void Update(float deltaTime) {
        if (RemainingInvincibility > 0.0f) {
            RemainingInvincibility -= deltaTime;
        }
        RegenerateHealth(deltaTime);
    }

    // Проверка, умер ли объект
    bool IsDeadState() const {
        return IsDead;
    }

    // Обработка смерти
    void OnDeath(GameObject* target) {
        if (!target) {
            std::cerr << "Error: Target is null in OnDeath!" << std::endl;
            return;
        }

        // Получаем позицию умирающего объекта
        auto collider = target->GetComponent<ColliderComponent>();
        auto transform = target->GetComponent<TransformComponent>();
        

        if (!transform) {
            std::cerr << "Error: TransformComponent not found in target object!" << std::endl;
            return;
        }

        // Загружаем текстуру опыта через TextureLoader
        SDL_Texture* experienceTexture = TextureLoader::GetInstance().LoadTexture("mana.png", ManagerGame::renderer);

        if (!experienceTexture) {
            std::cerr << "Failed to load experience texture!" << std::endl;
            return;
        }

        SDL_FPoint pos = { transform->Position.x + collider->OffsetColliderX, transform->Position.y + collider->OffsetColliderY };

        // Создаем новый объект опыта
        std::shared_ptr<GameObject> experience = std::dynamic_pointer_cast<GameObject>(std::make_shared <Experience>(experienceTexture, pos));

        ManagerGame::experience.push_back(experience);
        ManagerGame::objects.push_back(experience);

        std::cout << "Experience created at (" << transform->Position.x << ", " << transform->Position.y << ")!" << std::endl;

        // Деактивируем мёртвый объект
        target->GetComponent<StateComponent>()->IsActive = false;
    }

};