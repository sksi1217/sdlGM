#include "Skelet.h"
#include <iostream>

// Инициальзация
Skelet::Skelet(const SDL_FPoint& startPosition, SDL_Texture* texture) {
	// TransformComponent: Начальная позиция и масштаб
	auto transform = std::make_shared<TransformComponent>();
	transform->Position = startPosition;
	transform->Origin = { 0, 0 };
	AddComponent(transform);

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	AddComponent(movement);

	// StateComponent: Активность и коллизии
	auto state = std::make_shared<StateComponent>();
	// state->IsStatic = true;
	AddComponent(state);

	// PhysicsComponent: Масса и сила отталкивания
	auto physics = std::make_shared<PhysicsComponent>();
	AddComponent(physics);

	// RenderComponent: Текстура и цвет
	auto render = std::make_shared<RenderComponent>();
	render->Texture = texture;
	if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;
	AddComponent(render);

	// AnimationComponent: Настройка анимации
	auto animation = std::make_shared<AnimationComponent>();
	animation->animation = std::make_shared<Animation>(16, 16, 8, 1.0f / (movement->Speed * 0.2f));
	AddComponent(animation);

	// ColliderComponent
	auto collider = std::make_shared<ColliderComponent>();
	collider->OffsetColliderX = 0; // Смещение коллайдера по X
	collider->OffsetColliderY = 0; // Смещение коллайдера по Y
	collider->WidthColliderX = 16;  // Ширина коллайдера
	collider->HeightColliderY = 16; // Высота коллайдера
	AddComponent(collider);
}

void Skelet::Update(float deltaTime) {
	auto collider = GetComponent<ColliderComponent>();
	auto transform = GetComponent<TransformComponent>();

	collider->UpdatePosition(transform->Position);
}