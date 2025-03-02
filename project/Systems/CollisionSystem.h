#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include "../Core/GameObject.h"
#include "../Resources/ManagerGame.h"

class CollisionSystem {
public:
    void Update();

    // Функция проверки столкновения двух прямоугольников
    bool CheckCollision(GameObject* objA, GameObject* objB) {
        auto colliderA = objA->GetComponent<ColliderComponent>();
        auto colliderB = objB->GetComponent<ColliderComponent>();

        return (colliderA->Collider.x < colliderB->Collider.x + colliderB->Collider.w &&
            colliderA->Collider.x + colliderA->Collider.w > colliderB->Collider.x &&
            colliderA->Collider.y < colliderB->Collider.y + colliderB->Collider.h &&
            colliderA->Collider.y + colliderA->Collider.h > colliderB->Collider.y);
    }

    // Генерация сетки для оптимизации коллизий
    std::unordered_map<int, std::vector<GameObject*>> GenerateGrid(int cellSize, int width, int height) {
        std::unordered_map<int, std::vector<GameObject*>> grid;

        for (size_t i = 0; i < ManagerGame::objects.size(); ++i) {
            auto& obj = ManagerGame::objects[i]; // Берем ссылку на объект

            auto colliderObj = obj->GetComponent<ColliderComponent>();

            int minX = colliderObj->Collider.x / cellSize;
            int maxX = (colliderObj->Collider.x + colliderObj->Collider.w - 1) / cellSize;
            int minY = colliderObj->Collider.y / cellSize;
            int maxY = (colliderObj->Collider.y + colliderObj->Collider.h - 1) / cellSize;

            for (int y = minY; y <= maxY; ++y) {
                for (int x = minX; x <= maxX; ++x) {
                    int cellIndex = y * (width / cellSize) + x;
                    grid[cellIndex].push_back(ManagerGame::objects[i].get());
                }
            }
        }

        return grid;
    }

    // Решение коллизии
    void ResolveCollision(GameObject* objA, GameObject* objB) {
        auto transformA = objA->GetComponent<TransformComponent>();
        auto colliderA = objA->GetComponent<ColliderComponent>();
        auto physicsA = objA->GetComponent<PhysicsComponent>();

        auto transformB = objB->GetComponent<TransformComponent>();
        auto colliderB = objB->GetComponent<ColliderComponent>();
        auto physicsB = objB->GetComponent<PhysicsComponent>();

        if (!transformA || !colliderA || !transformB || !colliderB) return;

        int overlapX = std::min(
            colliderA->Collider.x + colliderA->Collider.w - colliderB->Collider.x,
            colliderB->Collider.x + colliderB->Collider.w - colliderA->Collider.x
        );

        int overlapY = std::min(
            colliderA->Collider.y + colliderA->Collider.h - colliderB->Collider.y,
            colliderB->Collider.y + colliderB->Collider.h - colliderA->Collider.y
        );

        if (!physicsB || physicsB->IsStatic) {
            // Корректируем только objA
            if (overlapX < overlapY) {
                if (colliderA->Collider.x < colliderB->Collider.x) {
                    transformA->Position.x -= overlapX;
                }
                else {
                    transformA->Position.x += overlapX;
                }
            }
            else if (overlapX > overlapY) {
                if (colliderA->Collider.y < colliderB->Collider.y) {
                    transformA->Position.y -= overlapY;
                }
                else {
                    transformA->Position.y += overlapY;
                }
            }
        }
        else {
            // Если objB не неподвижен, корректируем позиции обоих объектов
            if (overlapX < overlapY) {
                if (colliderA->Collider.x < colliderB->Collider.x) {
                    transformA->Position.x -= overlapX / 2;
                    transformB->Position.x += overlapX / 2;
                }
                else {
                    transformA->Position.x += overlapX / 2;
                    transformB->Position.x -= overlapX / 2;
                }
            }
            else if (overlapX > overlapY) {
                if (colliderA->Collider.y < colliderB->Collider.y) {
                    transformA->Position.y -= overlapY / 2;
                    transformB->Position.y += overlapY / 2;
                }
                else {
                    transformA->Position.y += overlapY / 2;
                    transformB->Position.y -= overlapY / 2;
                }
            }
        }

        // Обнуляем скорость для предотвращения повторных столкновений
        if (physicsA) physicsA->Velocity = { 0.0f, 0.0f };
        if (physicsB) physicsB->Velocity = { 0.0f, 0.0f };

        // Обновляем коллайдеры после изменения позиций
        colliderA->UpdatePosition(transformA->Position);
        colliderB->UpdatePosition(transformB->Position);
    }
};