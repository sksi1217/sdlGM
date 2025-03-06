#pragma once

#include "../Core/EntityHeaders.h" // Включаем заголовочные файлы для всех компонентов
#include <SDL.h>                   // Для SDL_FPoint

class Experience : public GameObject {
public:
    Experience(SDL_Texture* texture, const SDL_FPoint& position);

    int m_UpExperience = 10; // Количество опыта

};