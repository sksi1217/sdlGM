#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../Utils/MathUtils.h"
#include "../Core/Component.h"

#include "Camera.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <SDL.h>

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



    void DrawHealthBar(SDL_Renderer* renderer, float currentHealth, float maxHealth, int x, int y, int width, int height) {
        // Цвет фона полосы (серый)
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect background = { x, y, width, height };
        SDL_RenderFillRect(renderer, &background);

        // Цвет текущего здоровья (зеленый)
        float healthRatio = currentHealth / maxHealth;
        int healthWidth = static_cast<int>(width * healthRatio);
        SDL_Rect health = { x, y, healthWidth, height };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &health);

        // Опциональная рамка
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &background);
    }

    void DrawExperienceBar(SDL_Renderer* renderer, float currentHealth, float maxHealth, int x, int y, int width, int height) {
        // Цвет фона полосы (серый)
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_Rect background = { x, y, width, height };
        SDL_RenderFillRect(renderer, &background);

        // Цвет текущего здоровья (зеленый)
        float healthRatio = currentHealth / maxHealth;
        int healthWidth = static_cast<int>(width * healthRatio);
        SDL_Rect health = { x, y, healthWidth, height };
        SDL_SetRenderDrawColor(renderer, 53, 124, 255, 255);
        SDL_RenderFillRect(renderer, &health);

        // Опциональная рамка
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &background);
    }

private:
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