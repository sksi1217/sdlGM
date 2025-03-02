#pragma once
#include <vector>
#include <memory>
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
            // auto colliderB = objB->GetComponent<ColliderComponent>();

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
        auto transformB = objB->GetComponent<TransformComponent>();

        auto colliderA = objA->GetComponent<ColliderComponent>();
        auto colliderB = objB->GetComponent<ColliderComponent>();

        auto physicsA = objA->GetComponent<PhysicsComponent>();
        auto physicsB = objB->GetComponent<PhysicsComponent>();

        int overlapX = 0;
        int overlapY = 0;

        overlapX = std::min(colliderA->Collider.x + colliderA->Collider.w - colliderB->Collider.x, colliderB->Collider.x + colliderB->Collider.w - colliderA->Collider.x);
        overlapY = std::min(colliderA->Collider.y + colliderA->Collider.h - colliderB->Collider.y, colliderB->Collider.y + colliderB->Collider.h - colliderA->Collider.y);

        if (overlapX < overlapY) {
            if (colliderA->Collider.x < colliderB->Collider.x) {
                colliderA->Collider.x -= overlapX; // Коллизия слева
                transformA->Position.x = colliderA->Collider.x - colliderA->OffsetColliderX;
            }
            else {
                colliderA->Collider.x += overlapX; // Коллизия справа
                transformA->Position.x = colliderA->Collider.x - colliderA->OffsetColliderX;
            }
        }
        else if (overlapX > overlapY) {
            if (colliderA->Collider.y < colliderB->Collider.y) {
                colliderA->Collider.y -= overlapY; // Коллизия сверху
                transformA->Position.y = colliderA->Collider.y - colliderA->OffsetColliderY;
            }
            else {
                colliderA->Collider.y += overlapY; // Коллизия снизу
                transformA->Position.y = colliderA->Collider.y - colliderA->OffsetColliderY;
            }
        }

        // Корректируем позицию объекта A
        
        

        physicsB->Velocity.x = -physicsB->Velocity.x;
        physicsB->Velocity.y = -physicsB->Velocity.y;
    }
};