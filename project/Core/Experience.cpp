#include "Experience.h"

Experience::Experience(SDL_Texture* texture, const SDL_FPoint& position) {
    // TransformComponent: Начальная позиция и масштаб
    auto transform = std::make_shared<TransformComponent>();
    transform->Origin = { 8, 8 };
    transform->Scale = 0.5;
    transform->Position = position;
    AddComponent(transform);

    auto movement = std::make_shared<MovementComponent>();
    movement->m_movementSpeed = 50;
    AddComponent(movement);

    auto physics = std::make_shared<PhysicsComponent>();
    AddComponent(physics);

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

    // RenderComponent: Текстура опыта
    auto render = std::make_shared<RenderComponent>();
    render->Texture = texture;
    AddComponent(render);

    // AnimationComponent: Настройка анимации
    auto animation = std::make_shared<AnimationComponent>();
    animation->animation = std::make_shared<Animation>(16, 16, 1, 1.0f);
    AddComponent(animation);

    
}

void Experience::MoveTowards(SDL_FPoint playerTransform, float deltaTime)
{
    auto animationComponent = GetComponent<AnimationComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto collider = GetComponent<ColliderComponent>();

    if (!transform || !animationComponent || !collider) return;

    direction = {
        playerTransform.x - transform->Position.x,
        playerTransform.y - transform->Position.y
    };

    direction = MathUtils::Normalize(direction);

    HandleMovement(deltaTime);

    if (animationComponent && animationComponent->animation) animationComponent->animation->Update(true, static_cast<Uint32>(deltaTime * 1000.0f));
}

void Experience::HandleMovement(float deltaTime) {
    auto movement = GetComponent<MovementComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto physics = GetComponent<PhysicsComponent>();
    auto collider = GetComponent<ColliderComponent>();

    if (!transform || !movement || !physics) return;

    // Обновление Velocity на основе направления движения
    physics->Velocity.x = direction.x * movement->m_movementSpeed;
    physics->Velocity.y = direction.y * movement->m_movementSpeed;

    // Применение Velocity к позиции объекта
    transform->Position.x += physics->Velocity.x * deltaTime;
    transform->Position.y += physics->Velocity.y * deltaTime;

    // Обновление коллайдера
    if (collider) collider->UpdatePosition(transform->Position);
}