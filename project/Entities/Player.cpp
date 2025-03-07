#include "Player.h"
#include <iostream>

// Инициальзация
Player::Player(const SDL_FPoint& startPosition, SDL_Texture* texture) {
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

	auto energy = std::make_shared<EnergyComponent>();
	AddComponent(energy);

	auto ability = std::make_shared<AbilityComponent>();
	AddComponent(ability);

	auto gathering = std::make_shared<ResourceGatheringComponent>();
	AddComponent(gathering);

	auto level = std::make_shared<LevelComponent>();
	AddComponent(level);

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	movement->m_movementSpeed = 50;
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
	animation->animation = std::make_shared<Animation>(16, 16, 8, 1.0f / (movement->m_movementSpeed * 0.2f));
	AddComponent(animation);

	// ColliderComponent
	auto collider = std::make_shared<ColliderComponent>();
	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
	collider->OffsetColliderX = 8; // Смещение коллайдера по X
	collider->OffsetColliderY = 14; // Смещение коллайдера по Y
	collider->CircleRadius = 2; // Радиус круга
	collider->m_layer = ColliderComponent::Layer::Player;
	AddComponent(collider);
}

void Player::Update(float deltaTime) {
	auto animationComponent = GetComponent<AnimationComponent>();
	auto collider = GetComponent<ColliderComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto movement = GetComponent<MovementComponent>();
	auto physics = GetComponent<PhysicsComponent>();
	auto healt = GetComponent<HealthComponent>();

	if (!transform || !movement || !physics) return;

	// Сброс начальной скорости
	physics->Velocity = { 0.0f, 0.0f };

	// Обработка ввода с клавиатуры
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	HandleMovement(keyboardState, physics->Velocity, deltaTime);

	// Обновление позиции с учетом времени
	transform->Position = MathUtils::Add(transform->Position,
		MathUtils::Multiply(physics->Velocity, movement->m_movementSpeed * deltaTime));;

	// Обновление коллайдера
	if (collider) collider->UpdatePosition(transform->Position);

	HandleWeaponInteraction(deltaTime);

	healt->Update(deltaTime);

	SDL_FPoint pos;
	pos.x = transform->Position.x + collider->OffsetColliderX;
	pos.y = transform->Position.y + collider->OffsetColliderY;

	ResourceGathering(pos, deltaTime);

	// Обновление анимации
	bool isMoving = (physics->Velocity.x != 0.0f || physics->Velocity.y != 0.0f);
	if (animationComponent && animationComponent->animation) {
		animationComponent->animation->Update(isMoving, static_cast<Uint32>(deltaTime * 1000.0f));
	}
}

void Player::ResourceGathering(SDL_FPoint& playerPosition, float deltaTime) {
	// 1. Получаем необходимые компоненты
	auto gathering = GetComponent<ResourceGatheringComponent>();
	auto level = GetComponent<LevelComponent>();
	auto transform = GetComponent<TransformComponent>();
	auto collider = GetComponent<ColliderComponent>();

	if (collider) collider->UpdatePosition(transform->Position);

	// 2. Проверяем обязательные компоненты
	if (!gathering || !level || !transform) return;

	// 3. Получаем позицию с учетом коллайдера
	SDL_FPoint colliderPos = {
		transform->Position.x + collider->OffsetColliderX,
		transform->Position.y + collider->OffsetColliderY
	};

	// 4. Обрабатываем каждый объект опыта
	for (const auto& obj : ManagerGame::experience) {
		auto exp = std::dynamic_pointer_cast<Experience>(obj);
		if (!exp) continue;

		auto expState = exp->GetComponent<StateComponent>();
		auto expTransform = exp->GetComponent<TransformComponent>();

		// Пропускаем неактивные/неподходящие объекты
		if (!expState || !expState->IsActive || !expTransform) continue;

		// 5. Проверяем дистанцию
		float distance = gathering->IsWithinRange(colliderPos, expTransform->Position);
		float gatherRadius = gathering->GetGatherRadius();
		float sucklingRadius = gathering->GetSucklingRadius();

		// 6. Логика взаимодействия
		if (distance <= sucklingRadius) {
			if (distance <= gatherRadius) {
				// Подбор опыта
				level->AddExperience(exp->m_UpExperience);
				expState->IsActive = false;
				std::cout << "Picked up " << exp->m_UpExperience << " experience!" << std::endl;
			}
			else {
				// Движение опыта к игроку
				exp->MoveTowards(colliderPos, deltaTime);
			}
		}
	}
}

void Player::HandleWeaponInteraction(float deltaTime) {
	// Увеличиваем прошедшее время
	ElapsedTime += deltaTime;

	if (ManagerGame::_allWeapons.size() > 0) {
		for (const auto& obj : ManagerGame::_allWeapons) { // Итерация по unique_ptr
			if (!obj) continue; // Проверка на null-указатель

			auto weapon = std::dynamic_pointer_cast<Weapon>(obj);

			// Находим ближайшего врага
			// weaponPtr->nearestEnemy = weaponPtr->FindNearestEnemy();

			// Выполняем выстрел
			if (weapon) {
				// Вызываем метод Shoot, если объект — оружие
				weapon->Shoot(this, deltaTime);
			}
		}
		ElapsedTime = 0;
	}
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

void Player::Draw(SDL_Renderer* renderer, const Camera& camera)
{
	GameObject::Draw(renderer, camera);

	auto healt = GetComponent<HealthComponent>();
	auto lvl = GetComponent<LevelComponent>();

	if (!healt) return;
	DrawHealthBar(renderer, healt->CurrentHealth, healt->MaxHealth, 10, 10, 100, 20);
	DrawExperienceBar(renderer, lvl->GetExperience(), lvl->GetExperienceToNextLevel(), 10, 40, 100, 20);
}