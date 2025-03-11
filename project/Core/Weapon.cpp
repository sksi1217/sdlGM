#include "Weapon.h"
#include "Projectile.h"
#include <iostream>


Weapon::Weapon() {
	texture = TextureLoader::GetInstance().LoadTexture("Atlas.png", ManagerGame::renderer);
    InitializeRenderComponent();
    InitializeWeaponComponent();
}

#pragma region Инициализация оружия
void Weapon::InitializeRenderComponent() {
	auto render = std::make_shared<RenderComponent>();
	render->Texture = texture;
	if (!render->Texture) {
		std::cerr << "Texture load failed: " << texture << std::endl;
	}
	AddComponent(render);
}
void Weapon::InitializeWeaponComponent() {
	auto weapon = std::make_shared<WeaponComponent>();
	weapon->m_damage = 1;
	weapon->m_criticalChance = 0.5f;
	weapon->m_range = 100;
	weapon->m_shells = 1;
	weapon->m_fireRate = 1.0f;
	weapon->m_bulletInterval = 0.3f;
	weapon->accuracy = 1;
	weapon->m_lifetimeBullet = 6;
	AddComponent(weapon);
}
#pragma endregion

void Weapon::Shoot(Player* player, float deltaTime) {
	auto weapon = GetComponent<WeaponComponent>();
	auto attributes = player->GetComponent<AttributesComponent>();

	m_nearestEnemy = FindNearestEnemy(player);

	if (!weapon || !attributes || !m_nearestEnemy) { return; }

	// Увеличиваем прошедшее время
	weapon->m_elapsedTime += deltaTime;

	// Проверяем, нужно ли начать залп
	if (!weapon->im_isFiringZalp &&
		weapon->m_elapsedTime >= weapon->m_fireRate)
	{
		weapon->StartBurst();
	}

	// Выстрелы внутри залпа
	if (weapon->im_isFiringZalp &&
		weapon->m_elapsedTime >= weapon->m_bulletInterval &&
		weapon->m_shellsToFire > 0) 
	{
		auto enemyTransform = m_nearestEnemy->GetComponent<TransformComponent>();

		if (!enemyTransform) return;

		SDL_FPoint enemyPosition = enemyTransform->Position;

		// Создаём новую пулю
		CreateProjectile(player, enemyPosition);

		// Уменьшаем количество оставшихся пуль
		weapon->m_shellsToFire--;

		// Сбрасываем таймер
		weapon->m_elapsedTime = 0;

		// Если все пули выпущены, завершаем залп
		if (weapon->m_shellsToFire <= 0) {
			weapon->EndBurst();
		}
	}
}


void Weapon::CreateProjectile(Player* player, const SDL_FPoint& target) {
	// Создаем новую пулю как GameObject
	std::shared_ptr<GameObject> projectile = std::make_shared<Projectile>();

	InitializeProjectileComponents(projectile, player, target);
	
	// Добавляем пулю в игровой мир
	ManagerGame::objects.push_back(projectile);
}


void Weapon::InitializeProjectileComponents(std::shared_ptr<GameObject>& projectile,
	Player* player,
	const SDL_FPoint& target) 
{
#pragma region Не нужные атрибуты
	auto playerTransform = player->GetComponent<TransformComponent>();
	SDL_FPoint playerPosition = playerTransform->Position;

	auto weapon = GetComponent<WeaponComponent>();
	if (!weapon || !playerTransform) { return; }

	auto render = std::make_shared<RenderComponent>();
	render->Texture = GetComponent<RenderComponent>()->Texture;
	if (!render->Texture) std::cerr << "Bullet texture is missing" << std::endl;
	projectile->AddComponent(render);

	projectile->AddComponent(std::make_shared<StateComponent>());

	auto animation = std::make_shared<AnimationComponent>();
	animation->SpriteRow = 1; // Строка в спрайт-листе
	animation->animation = std::make_shared<Animation>(16, 16, 8, 1.0f / 20.0f);
	projectile->AddComponent(animation);
#pragma endregion

#pragma region Для увеличения Projectile
	auto transform = std::make_shared<TransformComponent>();
	// Смещение относительно игрока (например, чтобы пуля вылетала из ствола)
	SDL_FPoint originOffset = { 8.0f, 14.0f };
	transform->Position = {
		playerPosition.x + originOffset.x,
		playerPosition.y + originOffset.y
	};
	transform->Origin = { 8.0f, 8.0f }; // Центр спрайта
	transform->Scale = 0.5f; // Масштаб пули
	projectile->AddComponent(transform);

	auto collider = std::make_shared<ColliderComponent>();
	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
	collider->CircleRadius = 1.5; // Радиус круга
	collider->m_layer = ColliderComponent::Layer::Bullet;
	projectile->AddComponent(collider);
#pragma endregion

#pragma region направление и скорость Projectile
	auto physics = std::make_shared<PhysicsComponent>();
	auto projectilePos = projectile->GetComponent<TransformComponent>()->Position;

	SDL_FPoint direction = MathUtils::Subtract(target, projectilePos);

	SDL_FPoint originOffsetEnemy = { 0, 0 };
	direction = {
		direction.x + originOffset.x,
		direction.y + originOffset.y
	};

	direction = MathUtils::Normalize(direction);
	/*
	// Добавляем случайное отклонение (учитываем точность оружия)
	if (weapon->accuracy < 1.0f) {
		static std::mt19937 gen(std::random_device{}());
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
		direction.x += dist(gen) * (1.0f - weapon->accuracy);
		direction.y += dist(gen) * (1.0f - weapon->accuracy);
		direction = MathUtils::Normalize(direction); // Нормализуем после добавления шума
	}
	*/
	physics->Velocity = direction;
	projectile->AddComponent(physics);

	auto movement = std::make_shared<MovementComponent>();
	movement->m_movementSpeed = 5;
	projectile->AddComponent(movement);
#pragma endregion

#pragma region Характеристики Projectile
	auto attributesPlr = player->GetComponent<AttributesComponent>();
	auto projectileComp = std::make_shared<ProjectileComponent>();
	projectileComp->owner = player; // Устанавливаем владельца
	projectileComp->types = { ProjectileComponent::Type::Straight };
	projectileComp->m_damage = weapon->m_damage;
	projectileComp->m_criticalChance = weapon->m_criticalChance;
	projectileComp->m_lifetimeBullet = weapon->m_lifetimeBullet;
	projectileComp->m_vampirism = attributesPlr->m_vampirism;
	projectile->AddComponent(projectileComp);
#pragma endregion
}


GameObject* Weapon::FindNearestEnemy(Player* player) {
	auto weapon = GetComponent<WeaponComponent>();
	if (!weapon) return nullptr;

	GameObject* nearestEnemy = nullptr;
	float minDistance = weapon->m_range;

	auto playerTransform = player->GetComponent<TransformComponent>();
	if (!playerTransform) return nullptr;
	SDL_FPoint playerPos = playerTransform->Position;

	for (const auto& obj : ManagerGame::enemies) {
		if (auto enemy = obj.get()) { // Используйте weak_ptr для избежания dangling pointers
			if (auto state = enemy->GetComponent<StateComponent>();
				state && state->IsActive) {

				auto enemyTransform = enemy->GetComponent<TransformComponent>();
				if (!enemyTransform) continue;

				float distance = MathUtils::Distance(playerPos, enemyTransform->Position);
				if (distance < minDistance) {
					minDistance = distance;
					nearestEnemy = enemy;
				}
			}
		}
	}
	return nearestEnemy;
}