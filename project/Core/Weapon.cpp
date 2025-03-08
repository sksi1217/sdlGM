#include "Weapon.h"
#include <iostream>
#include "Projectile.h"
#include <random>

// Инициальзация
Weapon::Weapon(SDL_Texture* texture) {
	// RenderComponent: Текстура и цвет
	auto render = std::make_shared<RenderComponent>();
	render->Texture = texture;
	if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;
	AddComponent(render);

	// Weapon Component
	auto weapon = std::make_shared<WeaponComponent>();
	weapon->m_shells = 5;

	weapon->m_fireRate = 1;
	weapon->m_bulletInterval = 0.4;

	weapon->m_damage = 60;
	weapon->m_criticalChance = 0.5;
	weapon->m_criticalMultiplier = 3.5;
	weapon->m_lifetimeBullet = 2;
	AddComponent(weapon);
}

void Weapon::Shoot(Player* player, float deltaTime) {
	auto textere = GetComponent<RenderComponent>();
	auto weapon = GetComponent<WeaponComponent>();
	auto attributes = player->GetComponent<AttributesComponent>();

	m_nearestEnemy = FindNearestEnemy(player);

	if (!textere || !m_nearestEnemy) { return; }

	// Увеличиваем прошедшее время
	weapon->m_elapsedTime += deltaTime;

	// Проверяем, нужно ли начать залп
	if (!weapon->im_isFiringZalp && weapon->m_elapsedTime >= weapon->m_fireRate - attributes->GetCooldownReduction()) {
		std::cout << weapon->m_elapsedTime << std::endl;
		std::cout << weapon->m_fireRate << std::endl;
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
	auto weapon = GetComponent<WeaponComponent>();
	if (!weapon) { return; }

	// Создаем новую пулю как GameObject
	std::shared_ptr<GameObject> bullet = std::make_shared<Projectile>();

	// RenderComponent: Текстура
	auto render = std::make_shared<RenderComponent>();
	render->Texture = texture;
	if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;
	bullet->AddComponent(render);


	// ColliderComponent
	auto collider = std::make_shared<ColliderComponent>();
	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
	collider->CircleRadius = 1.5; // Радиус круга
	collider->m_layer = ColliderComponent::Layer::Bullet;
	bullet->AddComponent(collider);

	// TransformComponent: Начальная позиция и масштаб
	auto transform = std::make_shared<TransformComponent>();
	SDL_FPoint originPos = { 8, 14 };
	transform->Position = position;
	transform->Position.x += originPos.x;
	transform->Position.y += originPos.y;
	transform->Origin = { 8, 8 };
	transform->Rotation = 0;
	transform->Scale = 0.5;
	bullet->AddComponent(transform);

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	movement->m_movementSpeed = 20;
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

	// Добавляем случайное отклонение на основе точности
	
	if (weapon->accuracy < 1.0f) {
		std::random_device rd; // Генератор случайных чисел
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

		// Генерируем случайные отклонения по осям X и Y
		float deviationX = dist(gen) * (1.0f - weapon->accuracy);
		float deviationY = dist(gen) * (1.0f - weapon->accuracy);

		// Применяем отклонение к направлению
		physics->Velocity.x += deviationX;
		physics->Velocity.y += deviationY;

		// Нормализуем вектор после добавления отклонения
		physics->Velocity = MathUtils::Normalize(physics->Velocity);
	}

	bullet->AddComponent(physics);

	// AnimationComponent: Настройка анимации
	auto animation = std::make_shared<AnimationComponent>();
	animation->SpriteRow = 1;
	animation->animation = std::make_shared<Animation>(16, 16, 8, 1 / (movement->m_movementSpeed * 0.2f));
	bullet->AddComponent(animation);

	auto projectile = std::make_shared<ProjectileComponent>();
	projectile->m_damage = weapon->m_damage;
	projectile->m_criticalChance = weapon->m_criticalChance;
	projectile->m_criticalMultiplier = weapon->m_criticalMultiplier;
	projectile->m_lifetimeBullet = weapon->m_lifetimeBullet;
	bullet->AddComponent(projectile);

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