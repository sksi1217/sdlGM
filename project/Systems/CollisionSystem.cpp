#include "CollisionSystem.h"
#include <iostream>

void CollisionSystem::Update() {
    auto grid = GenerateGrid(32, 800, 600);
    //int collisionChecks = 0; // Счетчик проверок

    for (const auto& [cellIndex, cellObjects] : grid) {
        for (size_t i = 0; i < cellObjects.size(); ++i) {
            for (size_t j = i + 1; j < cellObjects.size(); ++j) {
                //collisionChecks++;
                if (CheckCollision(cellObjects[i], cellObjects[j])) {
                    ResolveCollision(cellObjects[i], cellObjects[j]);
                }
            }
        }
    }

    //std::cout << "Number of collision checks: " << collisionChecks << std::endl;
}