#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "../Core/GameObject.h"
#include "../Resources/ManagerGame.h"

class CollisionSystem {
public:
	void Update();

	bool CheckCollision(GameObject* objA, GameObject* objB) {
		auto colliderA = objA->GetComponent<ColliderComponent>();
		auto colliderB = objB->GetComponent<ColliderComponent>();

		if (!colliderA || !colliderB) return false;

		switch (colliderA->Type) {
		case ColliderComponent::ColliderType::RECTANGLE:
			switch (colliderB->Type) {
			case ColliderComponent::ColliderType::RECTANGLE:
				// Прямоугольник-прямоугольник
				return (colliderA->Collider.x < colliderB->Collider.x + colliderB->Collider.w &&
					colliderA->Collider.x + colliderA->Collider.w > colliderB->Collider.x &&
					colliderA->Collider.y < colliderB->Collider.y + colliderB->Collider.h &&
					colliderA->Collider.y + colliderA->Collider.h > colliderB->Collider.y);

			case ColliderComponent::ColliderType::CIRCLE:
				// Прямоугольник-круг
				return CheckRectCircleCollision(colliderA.get(), colliderB.get());

			default:
				return false;
			}

		case ColliderComponent::ColliderType::CIRCLE:
			switch (colliderB->Type) {
			case ColliderComponent::ColliderType::RECTANGLE:
				// Круг-прямоугольник (симметричный случай)
				return CheckRectCircleCollision(colliderB.get(), colliderA.get());

			case ColliderComponent::ColliderType::CIRCLE:
				// Круг-круг
				{
					float dx = colliderA->CircleCollider.x - colliderB->CircleCollider.x;
					float dy = colliderA->CircleCollider.y - colliderB->CircleCollider.y;
					float distanceSquared = dx * dx + dy * dy;
					float radiusSum = colliderA->CircleRadius + colliderB->CircleRadius;
					return distanceSquared <= radiusSum * radiusSum;
				}
			default:
				return false;
			}

		default:
			return false;
		}
	}

	// Общая функция для проверки столкновения прямоугольника и круга
	bool CheckRectCircleCollision(ColliderComponent* rectCollider, ColliderComponent* circleCollider) {
		float closestX = std::clamp(circleCollider->CircleCollider.x, static_cast<float>(rectCollider->Collider.x), static_cast<float>(rectCollider->Collider.x + rectCollider->Collider.w));
		float closestY = std::clamp(circleCollider->CircleCollider.y, static_cast<float>(rectCollider->Collider.y), static_cast<float>(rectCollider->Collider.y + rectCollider->Collider.h));
		float dx = circleCollider->CircleCollider.x - closestX;
		float dy = circleCollider->CircleCollider.y - closestY;
		float distanceSquared = dx * dx + dy * dy;
		return distanceSquared <= circleCollider->CircleRadius * circleCollider->CircleRadius;
	}

	// Генерация сетки для оптимизации коллизий
	std::unordered_map<int, std::vector<GameObject*>> GenerateGrid(int cellSize, int width, int height) {
		std::unordered_map<int, std::vector<GameObject*>> grid;

		for (size_t i = 0; i < ManagerGame::objects.size(); ++i) {
			auto& obj = ManagerGame::objects[i];
			auto colliderObj = obj->GetComponent<ColliderComponent>();

			// if (!colliderObj || !obj->IsActive()) continue;

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

	// Решение коллизии
	void ResolveCollision(GameObject* objA, GameObject* objB) {
		auto transformA = objA->GetComponent<TransformComponent>();
		auto colliderA = objA->GetComponent<ColliderComponent>();
		auto physicsA = objA->GetComponent<PhysicsComponent>();

		auto transformB = objB->GetComponent<TransformComponent>();
		auto colliderB = objB->GetComponent<ColliderComponent>();
		auto physicsB = objB->GetComponent<PhysicsComponent>();

		if (!transformA || !colliderA || !transformB || !colliderB) return;

		switch (colliderA->Type) {
		case ColliderComponent::ColliderType::RECTANGLE:
			switch (colliderB->Type) {
			case ColliderComponent::ColliderType::RECTANGLE:
				ResolveRectRectCollision(objA, objB);
				break;

			case ColliderComponent::ColliderType::CIRCLE:
				ResolveRectCircleCollision(objA, objB);
				break;

			default:
				break;
			}
			break;

		case ColliderComponent::ColliderType::CIRCLE:
			switch (colliderB->Type) {
			case ColliderComponent::ColliderType::RECTANGLE:
				ResolveCircleRectCollision(objA, objB);
				break;

			case ColliderComponent::ColliderType::CIRCLE:
				ResolveCircleCircleCollision(objA, objB);
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	// Разрешение коллизии прямоугольник-прямоугольник
	void ResolveRectRectCollision(GameObject* objA, GameObject* objB) {
		auto transformA = objA->GetComponent<TransformComponent>();
		auto colliderA = objA->GetComponent<ColliderComponent>();
		auto physicsA = objA->GetComponent<PhysicsComponent>();

		auto transformB = objB->GetComponent<TransformComponent>();
		auto colliderB = objB->GetComponent<ColliderComponent>();
		auto physicsB = objB->GetComponent<PhysicsComponent>();

		int overlapX = std::min(
			colliderA->Collider.x + colliderA->Collider.w - colliderB->Collider.x,
			colliderB->Collider.x + colliderB->Collider.w - colliderA->Collider.x
		);

		int overlapY = std::min(
			colliderA->Collider.y + colliderA->Collider.h - colliderB->Collider.y,
			colliderB->Collider.y + colliderB->Collider.h - colliderA->Collider.y
		);

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
		else {
			if (colliderA->Collider.y < colliderB->Collider.y) {
				transformA->Position.y -= overlapY / 2;
				transformB->Position.y += overlapY / 2;
			}
			else {
				transformA->Position.y += overlapY / 2;
				transformB->Position.y -= overlapY / 2;
			}
		}

		colliderA->UpdatePosition(transformA->Position);
		colliderB->UpdatePosition(transformB->Position);
	}

	// Разрешение коллизии прямоугольник-круг
	void ResolveRectCircleCollision(GameObject* rectObj, GameObject* circleObj) {
		auto transformRect = rectObj->GetComponent<TransformComponent>();
		auto colliderRect = rectObj->GetComponent<ColliderComponent>();
		auto physicsRect = rectObj->GetComponent<PhysicsComponent>();

		auto transformCircle = circleObj->GetComponent<TransformComponent>();
		auto colliderCircle = circleObj->GetComponent<ColliderComponent>();
		auto physicsCircle = circleObj->GetComponent<PhysicsComponent>();

		float closestX = std::clamp(colliderCircle->CircleCollider.x, static_cast<float>(colliderRect->Collider.x), static_cast<float>(colliderRect->Collider.x + colliderRect->Collider.w));
		float closestY = std::clamp(colliderCircle->CircleCollider.y, static_cast<float>(colliderRect->Collider.y), static_cast<float>(colliderRect->Collider.y + colliderRect->Collider.h));

		float dx = colliderCircle->CircleCollider.x - closestX;
		float dy = colliderCircle->CircleCollider.y - closestY;

		if (dx == 0 && dy == 0) return; // Круг внутри прямоугольника

		float distance = std::sqrt(dx * dx + dy * dy);
		float overlap = colliderCircle->CircleRadius - distance;

		if (overlap > 0) {
			float nx = dx / distance;
			float ny = dy / distance;

			transformCircle->Position.x += nx * overlap;
			transformCircle->Position.y += ny * overlap;

			colliderCircle->UpdatePosition(transformCircle->Position);
		}
	}

	// Разрешение коллизии круг-прямоугольник
	void ResolveCircleRectCollision(GameObject* circleObj, GameObject* rectObj) {
		ResolveRectCircleCollision(rectObj, circleObj); // Симметричный случай
	}

	// Разрешение коллизии круг-круг
	void ResolveCircleCircleCollision(GameObject* circleA, GameObject* circleB) {
		auto transformA = circleA->GetComponent<TransformComponent>();
		auto colliderA = circleA->GetComponent<ColliderComponent>();
		auto physicsA = circleA->GetComponent<PhysicsComponent>();

		auto transformB = circleB->GetComponent<TransformComponent>();
		auto colliderB = circleB->GetComponent<ColliderComponent>();
		auto physicsB = circleB->GetComponent<PhysicsComponent>();

		float dx = colliderB->CircleCollider.x - colliderA->CircleCollider.x;
		float dy = colliderB->CircleCollider.y - colliderA->CircleCollider.y;

		float distance = std::sqrt(dx * dx + dy * dy);
		if (distance == 0) return; // Центры совпадают

		float radiusSum = colliderA->CircleRadius + colliderB->CircleRadius;
		if (distance < radiusSum) {
			float overlap = radiusSum - distance;

			float nx = dx / distance;
			float ny = dy / distance;

			transformA->Position.x -= nx * overlap / 2;
			transformB->Position.x += nx * overlap / 2;

			transformA->Position.y -= ny * overlap / 2;
			transformB->Position.y += ny * overlap / 2;

			colliderA->UpdatePosition(transformA->Position);
			colliderB->UpdatePosition(transformB->Position);
		}
	}
};