#pragma once

#include "../Components/EntitiesComponent.h"

class Entities : public GameObject
{
public:
	virtual void InitializeComponent(const SDL_FPoint& startPosition, SDL_Texture* texture)
	{
		auto transform = std::make_shared<TransformComponent>();
		AddComponent(transform);

		auto health = std::make_shared<HealthComponent>();
		AddComponent(health);

		auto attributes = std::make_shared<AttributesComponent>();
		AddComponent(attributes);

		auto movement = std::make_shared<MovementComponent>();
		AddComponent(movement);

		auto state = std::make_shared<StateComponent>();
		AddComponent(state);

		auto dead = std::make_shared<DeathComponent>();
		AddComponent(dead);

		auto physics = std::make_shared<PhysicsComponent>();
		AddComponent(physics);

		auto render = std::make_shared<RenderComponent>();
		AddComponent(render);

		auto animation = std::make_shared<AnimationComponent>();
		AddComponent(animation);

		auto enemyDamageComponent = std::make_shared<EnemyDamageComponent>();
		AddComponent(enemyDamageComponent);

		auto collider = std::make_shared<ColliderComponent>();
		AddComponent(collider);
	}
};