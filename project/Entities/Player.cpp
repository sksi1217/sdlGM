#include "Player.h"

void Player::Update(float deltaTime) {
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	SDL_FPoint velocity = { 0.0f, 0.0f };


	/*
	HandleMovementInput(keyboardState, velocity);

	Position.x += velocity.x * Speed * deltaTime;
	Position.y += velocity.y * Speed * deltaTime;

	ColliderPosition();

	HandleCollisions();

	HandleWeaponInteraction(deltaTime);

	bool isMoving = (velocity.x != 0.0f || velocity.y != 0.0f);
	animation->Update(isMoving, static_cast<Uint32>(deltaTime * 1000.0f));
	*/

}