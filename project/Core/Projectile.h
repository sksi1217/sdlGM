#pragma once

#include "../Core/EntityHeaders.h"
#include <SDL.h>

class Projectile : public GameObject {
public:
	Projectile() {};

	void Update(float deltaTime) override;

	void ApplyModifications(std::shared_ptr<TransformComponent> transform, std::shared_ptr<ProjectileComponent> projectile, float deltaTime);
	void ApplyRotationAroundTrajectory(std::shared_ptr<TransformComponent> transform, std::shared_ptr<ProjectileComponent> projectile, SDL_FPoint direction, float deltaTime);

};