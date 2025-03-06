#include "Experience.h"

Experience::Experience(SDL_Texture* texture, const SDL_FPoint& position) {
    // TransformComponent: Начальная позиция и масштаб
    auto transform = std::make_shared<TransformComponent>();
    transform->Origin = { 8, 8 };
    transform->Scale = 0.5;
    transform->Position = position;
    AddComponent(transform);

    auto collider = std::make_shared<ColliderComponent>();
    collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
    collider->OffsetColliderX = 0; // Смещение коллайдера по X
    collider->OffsetColliderY = 0; // Смещение коллайдера по Y
    collider->CircleRadius = 2; // Радиус круга
    collider->m_layer = ColliderComponent::Layer::Item;
    AddComponent(collider);

    // StateComponent: Активность объекта
    auto state = std::make_shared<StateComponent>();
    AddComponent(state);

    // AnimationComponent: Настройка анимации
    auto animation = std::make_shared<AnimationComponent>();
    animation->animation = std::make_shared<Animation>(16, 16, 1, 1.0f);
    AddComponent(animation);

    // RenderComponent: Текстура опыта
    auto render = std::make_shared<RenderComponent>();
    render->Texture = texture;
    AddComponent(render);
}