#include "Skelet.h"
#include <iostream>

// Инициальзация
Skelet::Skelet(const SDL_FPoint& startPosition, SDL_Texture* texture) {
	// TransformComponent: Начальная позиция и масштаб
	auto transform = std::make_shared<TransformComponent>();
	transform->Position = startPosition;
	AddComponent(transform);

	auto health = std::make_shared<HealthComponent>();
	AddComponent(health);

	auto attributes = std::make_shared<AttributesComponent>();
	AddComponent(attributes);

	auto combat = std::make_shared<CombatComponent>();
	AddComponent(combat);

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	movement->m_movementSpeed = 0;
	AddComponent(movement);

	// StateComponent: Активность и коллизии
	auto state = std::make_shared<StateComponent>();
	AddComponent(state);

	// PhysicsComponent: Масса и сила отталкивания
	auto physics = std::make_shared<PhysicsComponent>();
	physics->IsStatic = false;
	physics->Mass = 0.1;
	AddComponent(physics);

	// RenderComponent: Текстура и цвет
	auto render = std::make_shared<RenderComponent>();
	render->Texture = texture;
	if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;
	AddComponent(render);

	// AnimationComponent: Настройка анимации
	auto animation = std::make_shared<AnimationComponent>();
	animation->animation = std::make_shared<Animation>(16, 16, 8, 1.0f / (movement->m_movementSpeed * 0.2f));
	AddComponent(animation);

	// ColliderComponent
	auto collider = std::make_shared<ColliderComponent>();
	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
	collider->OffsetColliderX = 8; // Смещение коллайдера по X
	collider->OffsetColliderY = 14; // Смещение коллайдера по Y
	collider->CircleRadius = 2.7; // Радиус круга
	collider->m_layer = ColliderComponent::Layer::Enemy;
	AddComponent(collider);
}

void Skelet::Update(float deltaTime) {
	auto animationComponent = GetComponent<AnimationComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto collider = GetComponent<ColliderComponent>();
	auto healt = GetComponent<HealthComponent>();
	
	direction = { targetPosition.x - transform->Position.x, targetPosition.y - transform->Position.y };
	direction = MathUtils::Normalize(direction);

	if (std::abs(direction.x) > std::abs(direction.y)) animationComponent->SpriteRow = (direction.x > 0 ? RightRow : LeftRow);
	else animationComponent->SpriteRow = (direction.y > 0 ? DownRow : UpRow);

	HandleMovement(deltaTime);
	
	healt->Update(deltaTime);

	bool isMoving = (direction.x != 0.0f || direction.y != 0.0f);
	if (animationComponent && animationComponent->animation) animationComponent->animation->Update(isMoving, static_cast<Uint32>(deltaTime * 1000.0f));
}

void Skelet::HandleMovement(float deltaTime) {
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