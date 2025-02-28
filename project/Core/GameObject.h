#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/StateComponent.h"

#include "Camera.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <SDL.h>
#include "Component.h"

class GameObject {
public:
    std::unordered_map<std::string, std::shared_ptr<Component>> components;

    template <typename T>
    void AddComponent(std::shared_ptr<T> component) {
        components[typeid(T).name()] = component;
    }

    template <typename T>
    std::shared_ptr<T> GetComponent() {
        return std::static_pointer_cast<T>(components[typeid(T).name()]);
    }

    virtual void Update(float deltaTime);
    virtual void Draw(SDL_Renderer* renderer, const Camera& camera);

    void ResolveCollision(GameObject* other);
    bool CheckCollision(GameObject* other);

private:
    void ApplyRepulsion(GameObject* obj, GameObject* other, bool isXAxis, float direction);


    // Вычисление длины вектора
    float Length(const SDL_FPoint& vector) {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y);
    }

    // Нормализация вектора
    SDL_FPoint Normalize(const SDL_FPoint& vector) {
        float length = Length(vector);
        if (length > 0.0f) {
            return { vector.x / length, vector.y / length };
        }
        return { 0.0f, 0.0f }; // Возвращаем нулевой вектор, если длина равна нулю
    }

    // Скалярное умножение вектора на число
    SDL_FPoint Multiply(const SDL_FPoint& vector, float scalar) {
        return { vector.x * scalar, vector.y * scalar };
    }

    // Скалярное деление вектора на число
    SDL_FPoint Divide(const SDL_FPoint& vector, float scalar) {
        if (scalar != 0.0f) {
            return { vector.x / scalar, vector.y / scalar };
        }
        return { 0.0f, 0.0f }; // Избегаем деления на ноль
    }

    // Разность двух векторов
    SDL_FPoint Subtract(const SDL_FPoint& a, const SDL_FPoint& b) {
        return { a.x - b.x, a.y - b.y };
    }

    // Сумма двух векторов
    SDL_FPoint Add(const SDL_FPoint& a, const SDL_FPoint& b) {
        return { a.x + b.x, a.y + b.y };
    }

    float Distance(const SDL_FPoint& a, const SDL_FPoint& b) {
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
};

#endif // GAME_OBJECT_H