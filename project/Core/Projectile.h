#pragma once

#include "../Core/Component.h"
#include <SDL.h>

class Projectile : public GameObject {
public:
	Projectile();

	void Update(float deltaTime) override;
};