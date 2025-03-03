#include "MathUtils.h"
#include <random>

float MathUtils::Length(const SDL_FPoint& vector) {
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

SDL_FPoint MathUtils::Normalize(const SDL_FPoint& vector) {
    float length = Length(vector);
    if (length > 0.0f) {
        return { vector.x / length, vector.y / length };
    }
    return { 0.0f, 0.0f };
}

SDL_FPoint MathUtils::Multiply(const SDL_FPoint& vector, float scalar) {
    return { vector.x * scalar, vector.y * scalar };
}

SDL_FPoint MathUtils::Divide(const SDL_FPoint& vector, float scalar) {
    if (scalar != 0.0f) {
        return { vector.x / scalar, vector.y / scalar };
    }
    return { 0.0f, 0.0f };
}

SDL_FPoint MathUtils::Subtract(const SDL_FPoint& a, const SDL_FPoint& b) {
    return { a.x - b.x, a.y - b.y };
}

SDL_FPoint MathUtils::Add(const SDL_FPoint& a, const SDL_FPoint& b) {
    return { a.x + b.x, a.y + b.y };
}

float MathUtils::Distance(const SDL_FPoint& a, const SDL_FPoint& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float MathUtils::Dot(const SDL_FPoint& a, const SDL_FPoint& b) {
    return a.x * b.x + a.y * b.y;
}

SDL_FPoint MathUtils::Negate(const SDL_FPoint& vector) {
    return { -vector.x, -vector.y };
}

float MathUtils::NextFloat() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0f, 1.0f);
    return dis(gen);
}
