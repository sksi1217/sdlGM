#pragma once

#include "../../Core/Weapon.h"

class MastersKeeper : public Weapon
{
public:
	MastersKeeper();

	// Выстрел
	void Shoot(Player* player, float deltaTime) override;

protected:
	SDL_Texture* texture = nullptr;
	GameObject* m_nearestEnemy = nullptr; // Ближайший враг

	void InitializeWeaponComponent() override;
	void CreateProjectile(Player* player, const SDL_FPoint& target);
	void InitializeProjectileComponents(std::shared_ptr<GameObject>& projectile, Player* player, const SDL_FPoint& target) override;
};