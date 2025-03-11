/*
#include "MastersKeeper.h"
#include "ProjectileMastersKeeper.h"

MastersKeeper::MastersKeeper()
{
	texture = TextureLoader::GetInstance().LoadTexture("Atlas.png", ManagerGame::renderer);
	InitializeRenderComponent();
	InitializeWeaponComponent();
}

void MastersKeeper::InitializeWeaponComponent() {
	auto weapon = std::make_shared<WeaponComponent>();
	weapon->m_damage = 50;
	weapon->m_criticalChance = 0.5f;
	weapon->m_range = 10;
	weapon->m_shells = 1;
	weapon->m_fireRate = 5.0f;
	weapon->m_bulletInterval = 3;
	weapon->accuracy = 0;
	weapon->m_lifetimeBullet = 5;
	AddComponent(weapon);
}

void MastersKeeper::Shoot(Player* player, float deltaTime) {
	auto weapon = GetComponent<WeaponComponent>();
	auto attributes = player->GetComponent<AttributesComponent>();

	// m_nearestEnemy = FindNearestEnemy(player);

	if (!weapon || !attributes) { return; }

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
		
		// Создаём новую проджектайл
		CreateProjectile(player, SDL_FPoint{0, 0});

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

void MastersKeeper::CreateProjectile(Player* player, const SDL_FPoint& target) {
	// Создаем новую пулю как GameObject
	std::shared_ptr<GameObject> bullet = std::make_shared<ProjectileMastersKeeper>();

	InitializeProjectileComponents(bullet, player, target);


	// Добавляем пулю в игровой мир
	ManagerGame::objects.push_back(bullet);
}

void MastersKeeper::InitializeProjectileComponents(std::shared_ptr<GameObject>& bullet,
	Player* player,	const SDL_FPoint& target)
{
	auto playerTransform = player->GetComponent<TransformComponent>();

	SDL_FPoint playerPosition = playerTransform->Position;

	auto weapon = GetComponent<WeaponComponent>();
	if (!weapon || !playerTransform) { return; }


	auto render = std::make_shared<RenderComponent>();
	render->Texture = GetComponent<RenderComponent>()->Texture;
	if (!render->Texture) std::cerr << "Bullet texture is missing" << std::endl;
	bullet->AddComponent(render);

	auto collider = std::make_shared<ColliderComponent>();
	collider->SetColliderType(ColliderComponent::ColliderType::RECTANGLE); // Установка круглого коллайдера
	//collider->CircleRadius = 1.9f; // Радиус круга
	collider->WidthColliderX = 6;
	collider->HeightColliderY = 6;
	collider->m_layer = ColliderComponent::Layer::Bullet;
	bullet->AddComponent(collider);

	auto transform = std::make_shared<TransformComponent>();
	// Смещение относительно игрока (например, чтобы пуля вылетала из ствола)
	SDL_FPoint originOffset = { 8, 8 };
	transform->Position = {
		playerPosition.x + originOffset.x,
		playerPosition.y + originOffset.y
	};
	transform->Origin = { 4.0f, 4.0f }; // Центр спрайта
	transform->Scale = 0.6; // Масштаб пули
	bullet->AddComponent(transform);

	auto movement = std::make_shared<MovementComponent>();
	movement->m_movementSpeed = 80;
	bullet->AddComponent(movement);

	bullet->AddComponent(std::make_shared<StateComponent>());

	auto physics = std::make_shared<PhysicsComponent>();
	bullet->AddComponent(physics);

	auto animation = std::make_shared<AnimationComponent>();
	animation->SpriteRow = 3; // Строка в спрайт-листе
	animation->animation = std::make_shared<Animation>(16, 16, 1, 1.0f / 20.0f);
	bullet->AddComponent(animation);

	auto attributesPlr = player->GetComponent<AttributesComponent>();

	auto projectile = std::make_shared<ProjectileComponent>();
	projectile->m_range = weapon->m_range;
	projectile->m_remove_bullet = true;
	projectile->owner = player; // Устанавливаем владельца
	projectile->m_damage = weapon->m_damage;
	projectile->m_criticalChance = weapon->m_criticalChance;
	projectile->m_lifetimeBullet = weapon->m_lifetimeBullet;
	projectile->m_vampirism = attributesPlr->m_vampirism;
	bullet->AddComponent(projectile);
}
*/