#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "../Core/EntityHeaders.h"
#include "../Resources/ManagerGame.h"
#include "../Entities/Player.h"
#include "../Core/GameObject.h"

class Player;

class Weapon : public GameObject {
public:
	// Конструктор
	Weapon();

	// Выстрел
	virtual void Shoot(Player* player, float deltaTime);

protected:
	SDL_Texture* texture = nullptr;
	GameObject* m_nearestEnemy = nullptr; // Ближайший враг

	virtual void InitializeRenderComponent();
	virtual void InitializeWeaponComponent();
	virtual void InitializeProjectileComponents(std::shared_ptr<GameObject>& projectile, Player* player, const SDL_FPoint& target);

	// Создание пули
	virtual void CreateProjectile(Player* player, const SDL_FPoint& direction);

	virtual GameObject* FindNearestEnemy(Player* player);
};

#endif // WEAPON_H