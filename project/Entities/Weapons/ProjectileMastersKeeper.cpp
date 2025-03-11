#include "ProjectileMastersKeeper.h"
#include <cmath>

/*
void ProjectileMastersKeeper::Update(float deltaTime)
{
    auto weapon = GetComponent<WeaponComponent>();
    auto movement = GetComponent<MovementComponent>();
    auto projectile = GetComponent<ProjectileComponent>();
    auto transform = GetComponent<TransformComponent>();
    auto status = GetComponent<StateComponent>();
    auto collider = GetComponent<ColliderComponent>();
    auto animationComponent = GetComponent<AnimationComponent>();

    if (!status->IsActive) return;

    // Обновляем текущее время жизни
    projectile->m_elapsedTime += deltaTime;
    if (projectile->m_elapsedTime >= projectile->m_lifetimeBullet) {
        status->IsActive = false; // Пуля исчезает после истечения времени жизни
        projectile->m_elapsedTime = 0;
        return;
    }

    // Обновляем угол в градусах
    projectile->m_orbitAngleDegrees += movement->m_movementSpeed * deltaTime;

    // Конвертируем градусы в радианы
    float angleRadians = projectile->m_orbitAngleDegrees * (M_PI / 180.0f);

    // Вычисляем позицию на орбите
    if (projectile->owner) {
        SDL_FPoint playerPos = projectile->owner->GetComponent<TransformComponent>()->Position;

        SDL_FPoint Pos;
        Pos.x = playerPos.x + transform->Origin.x;
        Pos.y = playerPos.y + transform->Origin.y;

        transform->Position.x = Pos.x + projectile->m_range * std::cos(angleRadians);
        transform->Position.y = Pos.y + projectile->m_range * std::sin(angleRadians);
    }

    // Обновление коллайдера
    collider->UpdatePosition(transform->Position);

    if (animationComponent && animationComponent->animation) {
        animationComponent->animation->Update(true, static_cast<Uint32>(deltaTime * 1000.0f));
    }
}
*/