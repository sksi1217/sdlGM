#include "Player.h"
#include <iostream>

// Инициальзация
Player::Player(const SDL_FPoint& startPosition, SDL_Texture* texture) {
	// TransformComponent: Начальная позиция и масштаб
	auto transform = std::make_shared<TransformComponent>();
	transform->Position = startPosition;
	AddComponent(transform);

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	AddComponent(movement);

	// StateComponent: Активность и коллизии
	auto state = std::make_shared<StateComponent>();
	AddComponent(state);

	// PhysicsComponent: Масса и сила отталкивания
	auto physics = std::make_shared<PhysicsComponent>();
	// physics->Mass = 100;
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
	collider->OffsetColliderX = 6; // Смещение коллайдера по X
	collider->OffsetColliderY = 11; // Смещение коллайдера по Y
	collider->WidthColliderX = 4;  // Ширина коллайдера
	collider->HeightColliderY = 5; // Высота коллайдера
	AddComponent(collider);
}

void Player::Update(float deltaTime) {
	auto animationComponent = GetComponent<AnimationComponent>();
	auto collider = GetComponent<ColliderComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto movement = GetComponent<MovementComponent>();

	if (!transform || !movement) return;


	// SDL_FPoint velocity = { 0.0f, 0.0f };
	transform->Velocity = { 0.0f, 0.0f };

	
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	HandleMovement(keyboardState, transform->Velocity, deltaTime);


	transform->Position.x += transform->Velocity.x * movement->Speed * deltaTime;
	transform->Position.y += transform->Velocity.y * movement->Speed * deltaTime;

	collider->UpdatePosition(transform->Position);

	bool isMoving = (transform->Velocity.x != 0.0f || transform->Velocity.y != 0.0f);
	animationComponent->animation->Update(isMoving, static_cast<Uint32>(deltaTime * 1000.0f));

}

// Обработка движения
void Player::HandleMovement(const Uint8* keyboardState, SDL_FPoint& velocity, float deltaTime) {

	if (keyboardState[SDL_SCANCODE_W]) velocity.y -= 1.0f; // Вверх
	if (keyboardState[SDL_SCANCODE_S]) velocity.y += 1.0f; // Вниз
	if (keyboardState[SDL_SCANCODE_A]) velocity.x -= 1.0f; // Влево
	if (keyboardState[SDL_SCANCODE_D]) velocity.x += 1.0f; // Вправо

	if (velocity.x != 0.0f || velocity.y != 0.0f) {
		float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		velocity.x /= length;
		velocity.y /= length;

		UpdateSpriteRow(velocity);
	}
}

// Обновление спрайтов
void Player::UpdateSpriteRow(const SDL_FPoint& velocity) {
	auto animation = GetComponent<AnimationComponent>();

	if (std::abs(velocity.x) > std::abs(velocity.y)) {
		animation->SpriteRow = (velocity.x > 0.0f) ? RightRow : LeftRow;
	}
	else {
		animation->SpriteRow = (velocity.y > 0.0f) ? DownRow : UpRow;
	}
}