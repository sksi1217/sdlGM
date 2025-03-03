#include "Projectile.h"

Projectile::Projectile() { }

void Projectile::Update(float deltaTime)
{
	auto weapon = GetComponent<WeaponComponent>();
	auto collider = GetComponent<ColliderComponent>();
	auto status = GetComponent<StateComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto movement = GetComponent<MovementComponent>();
	auto physics = GetComponent<PhysicsComponent>();

	if (!status->IsActive) return;

	// Обновляем текущее время жизни
	weapon->m_elapsedTime += deltaTime;
	if (weapon->m_elapsedTime >= weapon->m_lifetimeBullet) {
		status->IsActive = false; // Пуля исчезает после истечения времени жизни
		weapon->m_elapsedTime = 0;
		return;
	}

	// Обновляем позицию пули
	transform->Position = MathUtils::Add(transform->Position,
		MathUtils::Multiply(physics->Velocity, movement->Speed * deltaTime));;

	// Обновление коллайдера
	if (collider) collider->UpdatePosition(transform->Position);
}