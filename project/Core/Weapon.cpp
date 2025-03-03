#include "Weapon.h"
#include <iostream>
#include "Projectile.h"

// Инициальзация
Weapon::Weapon(const SDL_FPoint& startPosition, SDL_Texture* texture) {
	// TransformComponent: Начальная позиция и масштаб
	auto transform = std::make_shared<TransformComponent>();
	transform->Position = startPosition;
	transform->Origin = { 8, 8 };
	AddComponent(transform);

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	movement->Speed = 10;
	AddComponent(movement);

	// StateComponent: Активность и коллизии
	auto state = std::make_shared<StateComponent>();
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
	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
	collider->OffsetColliderX = 8; // Смещение коллайдера по X
	collider->OffsetColliderY = 14; // Смещение коллайдера по Y
	collider->CircleRadius = 1.5f; // Радиус круга
	AddComponent(collider);

	// Weapon Component
	auto weapon = std::make_shared<WeaponComponent>();
	AddComponent(weapon);
}

// Метод для расчета урона
float Weapon::CalculateDamage() {
	auto weapon = GetComponent<WeaponComponent>();
	float totalDamage = weapon->m_damage;
	if (IsCriticalHit()) {
		totalDamage *= weapon->m_criticalMultiplier;
	}
	return totalDamage;
}

void Weapon::Shoot(SDL_FPoint& Position) {
	auto textere = GetComponent<RenderComponent>();

	// if (!textere) std::cerr << "Error texture bullet!" << std::endl; return;

	// m_nearestEnemy = FindNearestEnemy();

	CreateBullet(Position, { 0, 0 }, textere->Texture);
	std::cout << "Boom!" << std::endl;
}

void Weapon::Update(float deltaTime) {

}

void Weapon::CreateBullet(SDL_FPoint& position, const SDL_FPoint& target, SDL_Texture* texture) {
	// Создаем новую пулю как GameObject
	std::shared_ptr<GameObject> bullet = std::make_shared<Projectile>();

	// TransformComponent: Начальная позиция и масштаб
	auto transform = std::make_shared<TransformComponent>();
	position.y += 8;
	transform->Position = position;
	transform->Origin = { 0, 0 };
	transform->Scale = 0.2;
	bullet->AddComponent(transform);

	std::cout << "Position.x " << transform->Position.x << std::endl;
	std::cout << "Position.y " << transform->Position.y << std::endl;

	// MovementComponent: Скорость движения
	auto movement = std::make_shared<MovementComponent>();
	movement->Speed = 50;
	bullet->AddComponent(movement);

	// StateComponent: Активность и коллизии
	auto state = std::make_shared<StateComponent>();
	bullet->AddComponent(state);

	// PhysicsComponent: Масса и сила отталкивания
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
	animation->animation = std::make_shared<Animation>(16, 16, 1, 1.0f / (movement->Speed * 0.2f));
	bullet->AddComponent(animation);

	// ColliderComponent
	auto collider = std::make_shared<ColliderComponent>();
	collider->SetColliderType(ColliderComponent::ColliderType::CIRCLE); // Установка круглого коллайдера
	collider->OffsetColliderX = 8; // Смещение коллайдера по X
	collider->OffsetColliderY = 8; // Смещение коллайдера по Y
	collider->CircleRadius = 3; // Радиус круга
	collider->m_layer = ColliderComponent::Layer::Bullet;
	bullet->AddComponent(collider);

	// 
	auto weapon = std::make_shared<WeaponComponent>();
	weapon->m_direction = direction;
	bullet->AddComponent(weapon);

	// Добавляем пулю в игровой мир
	ManagerGame::objects.push_back(bullet);
}


GameObject* Weapon::FindNearestEnemy() {
	/*
	GameObject* nearestEnemy = nullptr;
	float minDistance = std::numeric_limits<float>::max();

	for (const auto& obj : ManagerGame::enemies) {
		if (!obj || !obj->IsActive) continue; // Проверяем, что obj не null

		GameObject* enemy = obj.get(); // Получаем сырой указатель
		float distance = Help::Distance(StartPosition, enemy->Position);

		if (distance < Range && distance < minDistance) {
			minDistance = distance;
			nearestEnemy = enemy;
		}
	}

	return nearestEnemy;
	*/
	return nullptr;
}