#include "CollisionSystem.h"
#include <iostream>

void CollisionSystem::Update(std::vector<std::shared_ptr<GameObject>>& entities) {
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            if (entities[i]->CheckCollision(entities[j].get())) {
                entities[i]->ResolveCollision(entities[j].get());
            }
        }
    }
}