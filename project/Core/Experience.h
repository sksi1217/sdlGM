#pragma once

#include "../Core/EntityHeaders.h" // Включаем заголовочные файлы для всех компонентов
#include <SDL.h>                   // Для SDL_FPoint

class Experience : public GameObject {
public:
    Experience(SDL_Texture* texture, const SDL_FPoint& position);

    int m_UpExperience = 50; // Количество опыта

	void MoveTowards(SDL_FPoint playerTransform, float deltaTime);
	void HandleMovement(float deltaTime);

private:
	SDL_FPoint direction = { 0, 0 };
	SDL_FPoint targetPosition = { 0, 0 };
};