#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/StateComponent.h"

#include "../Utils/MathUtils.h"

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

    bool CheckCollision(GameObject* other);
    void ResolveCollision(GameObject* other);

private:
    float epsilon = 0.1f;

    // Функция для отрисовки круга
    void RenderCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, float cameraScale, const Camera& camera) {
        int scaledRadius = static_cast<int>(radius * cameraScale); // Масштабируем радиус
        int scaledCenterX = static_cast<int>((centerX - camera.GetPosition().x) * cameraScale);
        int scaledCenterY = static_cast<int>((centerY - camera.GetPosition().y) * cameraScale);

        if (scaledRadius <= 0) return;

        // Алгоритм Bresenham для рисования окружности
        int x = 0;
        int y = scaledRadius;
        int decisionOver2 = 1 - scaledRadius;   // Decision criterion divided by 2

        while (y >= x) {
            // Рисуем все восемь октантов одновременно
            SDL_RenderDrawPoint(renderer, scaledCenterX + x, scaledCenterY + y);
            SDL_RenderDrawPoint(renderer, scaledCenterX - x, scaledCenterY + y);
            SDL_RenderDrawPoint(renderer, scaledCenterX + x, scaledCenterY - y);
            SDL_RenderDrawPoint(renderer, scaledCenterX - x, scaledCenterY - y);
            SDL_RenderDrawPoint(renderer, scaledCenterX + y, scaledCenterY + x);
            SDL_RenderDrawPoint(renderer, scaledCenterX - y, scaledCenterY + x);
            SDL_RenderDrawPoint(renderer, scaledCenterX + y, scaledCenterY - x);
            SDL_RenderDrawPoint(renderer, scaledCenterX - y, scaledCenterY - x);

            x++;
            if (decisionOver2 > 0) {
                y--;
                decisionOver2 += 2 * (x - y) + 1;
            }
            else {
                decisionOver2 += 2 * x + 1;
            }
        }
    }
};

#endif // GAME_OBJECT_H