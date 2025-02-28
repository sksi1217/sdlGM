#include "CollisionSystem.h"

void CollisionSystem::Update() {
    for (size_t i = 0; i < ManagerGame::entities.size(); ++i) {
        for (size_t j = i + 1; j < ManagerGame::entities.size(); ++j) {
            if (ManagerGame::entities[i]->CheckCollision(ManagerGame::entities[j].get())) {
                ManagerGame::entities[i]->ResolveCollision(ManagerGame::entities[j].get());
            }
        }
    }
}