#pragma once

#include "../Core/EntityHeaders.h"

class DeathComponent : public Component
{
public:
    // Обработка смерти
    virtual void OnDeath(GameObject* target) {
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
        SDL_Texture* experienceTexture = TextureLoader::GetInstance().LoadTexture("Atlas.png", ManagerGame::renderer);

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