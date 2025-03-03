#include "CollisionSystem.h"
#include <iostream>

// Генерация сетки для оптимизации коллизий
std::unordered_map<int, std::vector<GameObject*>> CollisionSystem::GenerateGrid(int cellSize, int width, int height) {
	std::unordered_map<int, std::vector<GameObject*>> grid;

	for (size_t i = 0; i < ManagerGame::objects.size(); ++i) {
		auto& obj = ManagerGame::objects[i];
		auto colliderObj = obj->GetComponent<ColliderComponent>();

		if (!colliderObj) continue;

		int left, right, top, bottom;

		if (colliderObj->Type == ColliderComponent::ColliderType::RECTANGLE) {
			left = colliderObj->Collider.x;
			right = colliderObj->Collider.x + colliderObj->Collider.w;
			top = colliderObj->Collider.y;
			bottom = colliderObj->Collider.y + colliderObj->Collider.h;
		}
		else if (colliderObj->Type == ColliderComponent::ColliderType::CIRCLE) {
			left = colliderObj->CircleCollider.x - colliderObj->CircleRadius;
			right = colliderObj->CircleCollider.x + colliderObj->CircleRadius;
			top = colliderObj->CircleCollider.y - colliderObj->CircleRadius;
			bottom = colliderObj->CircleCollider.y + colliderObj->CircleRadius;
		}
		else {
			std::cout << "Nothing!" << std::endl;
			continue; // Неизвестный тип коллайдера
		}

		int minX = std::max(0, left / cellSize);
		int maxX = std::min((width / cellSize) - 1, (right - 1) / cellSize);
		int minY = std::max(0, top / cellSize);
		int maxY = std::min((height / cellSize) - 1, (bottom - 1) / cellSize);

		for (int y = minY; y <= maxY; ++y) {
			for (int x = minX; x <= maxX; ++x) {
				int cellIndex = y * (width / cellSize) + x;
				grid[cellIndex].push_back(obj.get());
			}
		}
	}

	return grid;
}

void CollisionSystem::Update() {
    auto grid = GenerateGrid(32, 800, 600); // Размер ячейки: 128
    int collisionChecks = 0;

    for (const auto& [cellIndex, cellObjects] : grid) {
        if (cellObjects.size() < 2) continue; // Пропускаем пустые или одиночные ячейки

        for (size_t i = 0; i < cellObjects.size(); ++i) {
            for (size_t j = i + 1; j < cellObjects.size(); ++j) {
                collisionChecks++;
                if (CheckCollision(cellObjects[i], cellObjects[j])) {
                    ResolveCollision(cellObjects[i], cellObjects[j]);
                }
            }
        }
    }

    std::cout << "Number of collision checks: " << collisionChecks << std::endl;
}

