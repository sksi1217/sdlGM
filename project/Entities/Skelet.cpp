#include "Skelet.h"
#include <iostream>

// Инициальзация
Skelet::Skelet(const SDL_FPoint& startPosition, SDL_Texture* texture, std::shared_ptr<TransformComponent> playerTransform): playerTransform(playerTransform) {	
	InitializeComponent(startPosition, texture);

	auto render = GetComponent<RenderComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto animation = GetComponent<AnimationComponent>();
	auto movement = GetComponent<MovementComponent>();
	auto collider = GetComponent<ColliderComponent>();

	render->Texture = texture;
	if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;

	transform->Position = startPosition;

	movement->m_movementSpeed = 0;

	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE);
	collider->OffsetColliderX = 8;
	collider->OffsetColliderY = 14;
	collider->CircleRadius = 2.7;
	collider->m_layer = ColliderComponent::Layer::Enemy;

	animation->animation = std::make_shared<Animation>(16, 16, 8, 1.0f / (movement->m_movementSpeed * 0.2f));

	auto healthComponent = GetComponent<HealthComponent>();
	if (healthComponent) {
		healthComponent->m_currentHealth = healthComponent->m_maxHealth;
	}
}

void Skelet::Update(float deltaTime) {
	auto animationComponent = GetComponent<AnimationComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto collider = GetComponent<ColliderComponent>();
	auto healt = GetComponent<HealthComponent>();
	
	if (!transform || !playerTransform || !animationComponent || !collider || !healt) return;

	direction = { 
		playerTransform->Position.x - transform->Position.x,
		playerTransform->Position.y - transform->Position.y 
	};

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