#pragma once
#include <SDL.h>
#include <cmath>

namespace MathUtils {
    float Length(const SDL_FPoint& vector);
    SDL_FPoint Normalize(const SDL_FPoint& vector);
    SDL_FPoint Multiply(const SDL_FPoint& vector, float scalar);
    SDL_FPoint Divide(const SDL_FPoint& vector, float scalar);
    SDL_FPoint Subtract(const SDL_FPoint& a, const SDL_FPoint& b);
    SDL_FPoint Add(const SDL_FPoint& a, const SDL_FPoint& b);
    float Distance(const SDL_FPoint& a, const SDL_FPoint& b);
    // Скалярное произведение двух векторов
    float Dot(const SDL_FPoint& a, const SDL_FPoint& b);

    SDL_FPoint Negate(const SDL_FPoint& vector);
}