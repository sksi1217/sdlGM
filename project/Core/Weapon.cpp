#include "Weapon.h"
#include <iostream>
#include "Projectile.h"

// Инициальзация
Weapon::Weapon(SDL_Texture* texture) {
	// RenderComponent: Текстура и цвет
	auto render = std::make_shared<RenderComponent>();
	render->Texture = texture;
	if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;
	AddComponent(render);

	// Weapon Component
	auto weapon = std::make_shared<WeaponComponent>();
	AddComponent(weapon);
}

void Weapon::Shoot(Player* player, float deltaTime) {
	auto textere = GetComponent<RenderComponent>();
	auto weapon = GetComponent<WeaponComponent>();

	m_nearestEnemy = FindNearestEnemy(player);

	if (!textere || !m_nearestEnemy) { std::cerr << "Error texture bullet!\nOr Not Nearest Enemy" << std::endl; return; }

	// Увеличиваем прошедшее время
	weapon->m_elapsedTime += deltaTime;

	// Проверяем, нужно ли начать залп
	if (!weapon->im_isFiringZalp && weapon->m_elapsedTime >= weapon->m_fireRate) {
		
		weapon->im_isFiringZalp = true;
		weapon->m_shellsToFire = weapon->m_shells; // Количество пуль в залпе
		weapon->m_elapsedTime = 0; // Сбрасываем таймер
	}

	// Выстрелы внутри залпа
	if (weapon->im_isFiringZalp && weapon->m_elapsedTime >= weapon->m_bulletInterval && weapon->m_shellsToFire > 0) {
		// Создаём новую пулю
		CreateBullet(player->GetComponent<TransformComponent>()->Position, m_nearestEnemy->GetComponent<TransformComponent>()->Position, textere->Texture);

		// Уменьшаем количество оставшихся пуль
		weapon->m_shellsToFire--;

		// Сбрасываем таймер
		weapon->m_elapsedTime = 0;
	}

	// Если все пули выпущены, завершаем залп
	if (weapon->m_shellsToFire <= 0) {
		weapon->im_isFiringZalp = false;
	}
}

void Weapon::CreateBullet(SDL_FPoint& position, const SDL_FPoint& target, SDL_Texture* texture) {
	// Создаем новую пулю как GameObject
	std::shared_ptr<GameObject> bullet = std::make_shared<Projectile>();

	// ColliderComponent
	auto collider = std::make_shared<ColliderComponent>();
	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
	collider->OffsetColliderX = 8; // Смещение коллайдера по X
	collider->OffsetColliderY = 8; // Смещение коллайдера по Y
	collider->CircleRadius = 1.5; // Радиус круга
	collider->m_layer = ColliderComponent::Layer::Bullet;
	bullet->AddComponent(collider);

	// TransformComponent: Начальная позиция и масштаб
	auto transform = std::make_shared<TransformComponent>();
	SDL_FPoint originPos = { 0, 6 };
	transform->Position = position;
	transform->Position.x += originPos.x;
	transform->Position.y += originPos.y;
	transform->Origin = { 0, 0 };
	transform->Rotation = 0;
	transform->Scale = 1;
	bullet->AddComponent(transform);

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	movement->m_movementSpeed = 100;
	bullet->AddComponent(movement);

	// StateComponent
	bullet->AddComponent(std::make_shared<StateComponent>());

	// PhysicsComponent
	auto physics = std::make_shared<PhysicsComponent>();
	// Вектор направления: разница между позицией цели и позицией пули
	SDL_FPoint direction = MathUtils::Subtract(target, position);
	float length = MathUtils::Length(direction);
	if (length > 0.0f) {
		physics->Velocity = MathUtils::Normalize(direction);
	}
	else {
		physics->Velocity = { 0.0f, 0.0f };
	}
	bullet->AddComponent(physics);

	// RenderComponent: Текстура и цвет
	auto render = std::make_shared<RenderComponent>();
	render->Texture = texture;
	if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;
	bullet->AddComponent(render);

	// AnimationComponent: Настройка анимации
	auto animation = std::make_shared<AnimationComponent>();
	animation->animation = std::make_shared<Animation>(16, 16, 8, 1 / (movement->m_movementSpeed * 0.2f));
	bullet->AddComponent(animation);

	// WeaponComponent
	auto weapon = std::make_shared<WeaponComponent>();
	weapon->m_damage = 100;
	weapon->m_criticalChance = 0.5;
	weapon->m_remove_bullet = true;
	weapon->m_direction = direction;
	bullet->AddComponent(weapon);

	// Добавляем пулю в игровой мир
	ManagerGame::objects.push_back(bullet);
}


GameObject* Weapon::FindNearestEnemy(Player* player) {
	auto weapon = GetComponent<WeaponComponent>();

	GameObject* nearestEnemy = nullptr;
	float minDistance = std::numeric_limits<float>::max();

	for (const auto& obj : ManagerGame::enemies) {
		auto transform = obj->GetComponent<TransformComponent>();
		auto state = obj->GetComponent<StateComponent>();

		if (!obj || !state->IsActive) continue; // Проверяем, что obj не null

		float distance = MathUtils::Distance(player->GetComponent<TransformComponent>()->Position, transform->Position);

		if (distance < weapon->m_range && distance < minDistance) {
			minDistance = distance;
			nearestEnemy = obj.get();
		}
	}

	return nearestEnemy;
}