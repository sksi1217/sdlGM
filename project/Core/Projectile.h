#pragma once

#include "../Core/EntityHeaders.h"
#include <SDL.h>

class Projectile : public GameObject {
public:
	Projectile();

	void Update(float deltaTime) override;
};