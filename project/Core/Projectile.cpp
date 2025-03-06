#include "Projectile.h"

Projectile::Projectile() { }

void Projectile::Update(float deltaTime)
{
    auto collider = GetComponent<ColliderComponent>();
	auto weapon = GetComponent<WeaponComponent>();
	
	auto status = GetComponent<StateComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto movement = GetComponent<MovementComponent>();
	auto physics = GetComponent<PhysicsComponent>();
	auto animationComponent = GetComponent<AnimationComponent>();

    if (!status->IsActive) return;

    // Обновляем текущее время жизни
    weapon->m_elapsedTime += deltaTime;
    if (weapon->m_elapsedTime >= weapon->m_lifetimeBullet) {
        status->IsActive = false; // Пуля исчезает после истечения времени жизни
        weapon->m_elapsedTime = 0;
        return;
    }

    // Применение Velocity к позиции объекта
    transform->Position.x += physics->Velocity.x * movement->m_movementSpeed * deltaTime;
    transform->Position.y += physics->Velocity.y * movement->m_movementSpeed * deltaTime;

    // Обновление коллайдера
    collider->UpdatePosition(transform->Position);

    if (animationComponent && animationComponent->animation) {
        animationComponent->animation->Update(true, static_cast<Uint32>(deltaTime * 1000.0f));
    }
}