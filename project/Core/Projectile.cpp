// Projectile.cpp
#include "Projectile.h"
#include <cmath>

void Projectile::Update(float deltaTime) {
	auto status = GetComponent<StateComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto movement = GetComponent<MovementComponent>();
	auto projectile = GetComponent<ProjectileComponent>();
	auto physics = GetComponent<PhysicsComponent>();
	auto collider = GetComponent<ColliderComponent>();
	auto animationComponent = GetComponent<AnimationComponent>();

	if (!status->IsActive) return;

	projectile->elapsedTime += deltaTime;

	std::cout << projectile->lifetime;

	if (projectile->elapsedTime >= projectile->lifetime) {
		status->IsActive = false;
		return;
	}

	bool hasPositional = projectile->HasPositionalType();
	SDL_FPoint direction = physics->Velocity;
	float speed = movement->m_movementSpeed;

	for (auto type : projectile->types) {
		switch (type) {
		case ProjectileComponent::Type::Straight:
			break; // Default behavior

		case ProjectileComponent::Type::ZigZag: {
			float phase = projectile->elapsedTime * (5 * M_PI) / projectile->zigzag.frequency;
			float offset = sin(phase) * projectile->zigzag.amplitude;
			SDL_FPoint perp = { -direction.y, direction.x };
			transform->Position.x += perp.x * offset * deltaTime;
			transform->Position.y += perp.y * offset * deltaTime;
			break;
		}

		case ProjectileComponent::Type::Circular: {
			float angle = projectile->elapsedTime * projectile->circular.angularSpeed * M_PI / 180.0f;
			transform->Position.x = projectile->circular.center.x + projectile->circular.radius * cos(angle);
			transform->Position.y = projectile->circular.center.y + projectile->circular.radius * sin(angle);
			break;
		}

		case ProjectileComponent::Type::Spiral: {
			float currentRadius = projectile->spiral.startRadius + projectile->spiral.expansionSpeed * projectile->elapsedTime;
			float angle = projectile->elapsedTime * projectile->spiral.angularSpeed * M_PI / 180.0f;
			transform->Position.x = projectile->spiral.center.x + currentRadius * cos(angle);
			transform->Position.y = projectile->spiral.center.y + currentRadius * sin(angle);
			break;
		}

		case ProjectileComponent::Type::Wave: {
			float phase = projectile->elapsedTime * (2 * M_PI) / projectile->wave.frequency;
			float offset = sin(phase) * projectile->wave.amplitude;
			if (projectile->wave.horizontal) {
				transform->Position.x += offset * deltaTime;
			}
			else {
				transform->Position.y += offset * deltaTime;
			}
			break;
		}

		case ProjectileComponent::Type::Chaotic: {
			projectile->chaotic.timer -= deltaTime;
			if (projectile->chaotic.timer <= 0) {
				float angle = (rand() % 360) * M_PI / 180.0f;
				projectile->chaotic.direction = { cos(angle), sin(angle) };
				projectile->chaotic.timer = projectile->chaotic.changeInterval;
			}
			direction = projectile->chaotic.direction;
			break;
		}

		case ProjectileComponent::Type::HyperAcceleration: {
			speed += projectile->hyperAccel.acceleration * projectile->elapsedTime;
			break;
		}

		case ProjectileComponent::Type::Orbital: {
			float angle = projectile->elapsedTime * projectile->orbital.angularSpeed * M_PI / 180.0f;
			transform->Position.x = projectile->orbital.center.x + projectile->orbital.radius * cos(angle);
			transform->Position.y = projectile->orbital.center.y + projectile->orbital.radius * sin(angle);
			break;
		}

		case ProjectileComponent::Type::Returning: {
			projectile->returning.timer += deltaTime;
			if (!projectile->returning.isReturning) {
				if (projectile->returning.timer >= projectile->returning.returnDelay) {
					projectile->returning.isReturning = true;
					SDL_FPoint ownerPos = projectile->owner->GetComponent<TransformComponent>()->Position;
					direction = MathUtils::Normalize(MathUtils::Subtract(ownerPos, transform->Position));
				}
			}
			speed = projectile->returning.returnSpeed;
			break;
		}
		default:
			break;
		}
	}

	// Сохраняем позицию после движения
	projectile->basePosition = transform->Position;

	// Применение модификаций смещения
	// ApplyModifications(transform, projectile, deltaTime);

// 3. Применяем вращение вокруг траектории
	ApplyRotationAroundTrajectory(transform, projectile, physics->Velocity, deltaTime);

	// Apply movement
	if (!hasPositional) {
		transform->Position.x += direction.x * speed * deltaTime;
		transform->Position.y += direction.y * speed * deltaTime;
	}
	// Обновление коллайдера
	collider->UpdatePosition(transform->Position);

	if (animationComponent && animationComponent->animation) {
		animationComponent->animation->Update(true, static_cast<Uint32>(deltaTime * 1000.0f));
	}
}

void Projectile::ApplyModifications(std::shared_ptr<TransformComponent> transform, std::shared_ptr<ProjectileComponent> projectile, float deltaTime) {
	// Вращение вокруг центра
	if (projectile->modificationParams.rotateAroundCenter) {
		float angle = projectile->elapsedTime * projectile->modificationParams.rotationSpeed * M_PI / 180.0f;
		float radius = std::sqrt(std::pow(transform->Position.x - projectile->circular.center.x, 2) +
			std::pow(transform->Position.y - projectile->circular.center.y, 2));
		transform->Position.x = projectile->circular.center.x + radius * cos(angle);
		transform->Position.y = projectile->circular.center.y + radius * sin(angle);
	}

	// Дополнительное смещение
	float phase = projectile->elapsedTime * (2 * M_PI) / projectile->modificationParams.offsetFrequency;
	float offset = sin(phase) * projectile->modificationParams.offsetAmplitude;
	transform->Position.x += offset * deltaTime;
	transform->Position.y += offset * deltaTime;
}

void Projectile::ApplyRotationAroundTrajectory(
	std::shared_ptr<TransformComponent> transform,
	std::shared_ptr<ProjectileComponent> projectile,
	SDL_FPoint direction,
	float deltaTime
) {
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length == 0) return;
	direction.x /= length;
	direction.y /= length;

	SDL_FPoint perpendicular = { -direction.y, direction.x };

	float angle = projectile->elapsedTime * projectile->rotation.angularSpeed * M_PI / 180.0f;

	float dx = projectile->rotation.radius * std::cos(angle);
	float dy = projectile->rotation.radius * std::sin(angle);

	// Добавляем смещение к текущей позиции
	transform->Position.x += perpendicular.x * dx;
	transform->Position.y += perpendicular.y * dy;
}