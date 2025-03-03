#include "Projectile.h"
#include <iostream>

Projectile::Projectile() { }

void Projectile::Update(float deltaTime) {
    auto weapon = GetComponent<WeaponComponent>();
    if (!weapon) {
        std::cerr << "Error: WeaponComponent is not initialized!" << std::endl;
        return;
    }

    auto collider = GetComponent<ColliderComponent>();
    if (!collider) {
        std::cerr << "Error: ColliderComponent is not initialized!" << std::endl;
        return;
    }

    auto status = GetComponent<StateComponent>();
    if (!status) {
        std::cerr << "Error: StateComponent is not initialized!" << std::endl;
        return;
    }

    auto transform = GetComponent<TransformComponent>();
    if (!transform) {
        std::cerr << "Error: TransformComponent is not initialized!" << std::endl;
        return;
    }

    auto movement = GetComponent<MovementComponent>();
    if (!movement) {
        std::cerr << "Error: MovementComponent is not initialized!" << std::endl;
        return;
    }

    auto physics = GetComponent<PhysicsComponent>();
    if (!physics) {
        std::cerr << "Error: PhysicsComponent is not initialized!" << std::endl;
        return;
    }

    if (!status->IsActive) return;

    // Обновляем текущее время жизни
    weapon->m_elapsedTime += deltaTime;
    if (weapon->m_elapsedTime >= weapon->m_lifetimeBullet) {
        status->IsActive = false; // Пуля исчезает после истечения времени жизни
        weapon->m_elapsedTime = 0;
        return;
    }

    weapon->m_direction = MathUtils::Normalize(weapon->m_direction);

    // Обновление Velocity на основе направления движения
    physics->Velocity.x = weapon->m_direction.x * movement->Speed;
    physics->Velocity.y = weapon->m_direction.y * movement->Speed;

    // Применение Velocity к позиции объекта
    transform->Position.x += physics->Velocity.x * deltaTime;
    transform->Position.y += physics->Velocity.y * deltaTime;

    // Обновление коллайдера
    collider->UpdatePosition(transform->Position);
}