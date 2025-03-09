#pragma once
#include "../../Core/Projectile.h"

class ProjectileMastersKeeper : public Projectile {
public:
	void Update(float deltaTime) override;
};