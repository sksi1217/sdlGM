#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "../Core/GameObject.h"
#include "../Resources/ManagerGame.h"

class CollisionSystem {
public:

	// Генерация сетки для оптимизации коллизий
	std::unordered_map<int, std::vector<GameObject*>> GenerateGrid(int cellSize, int width, int height);

	void Update();

	bool ShouldCollide(GameObject* objA, GameObject* objB) {
		auto colliderA = objA->GetComponent<ColliderComponent>();
		auto colliderB = objB->GetComponent<ColliderComponent>();

		if ((colliderA->m_layer == ColliderComponent::Layer::Player && colliderB->m_layer == ColliderComponent::Layer::Enemy) ||
			(colliderA->m_layer == ColliderComponent::Layer::Enemy && colliderB->m_layer == ColliderComponent::Layer::Player)) {
			return true; // Игрок и враг сталкиваются
		}

		if ((colliderA->m_layer == ColliderComponent::Layer::Player && colliderB->m_layer == ColliderComponent::Layer::Enemy) ||
			(colliderA->m_layer == ColliderComponent::Layer::Enemy && colliderB->m_layer == ColliderComponent::Layer::Player)) {
			return true; // Игрок и враг сталкиваются
		}

		if ((colliderA->m_layer == ColliderComponent::Layer::Bullet && colliderB->m_layer == ColliderComponent::Layer::Enemy) ||
			(colliderA->m_layer == ColliderComponent::Layer::Enemy && colliderB->m_layer == ColliderComponent::Layer::Bullet)) {
			return true; // Пуля и враг сталкиваются
		}

		if ((colliderA->m_layer == ColliderComponent::Layer::Player || colliderB->m_layer == ColliderComponent::Layer::Player) &&
			(colliderA->m_layer == ColliderComponent::Layer::Wall || colliderB->m_layer == ColliderComponent::Layer::Wall)) {
			return true; // Игрок сталкивается со стеной
		}


		return false; // По умолчанию не сталкиваются
	}

#pragma region Check Collision
	bool CheckRectRectCollision(ColliderComponent* a, ColliderComponent* b) {
		return (a->Collider.x < b->Collider.x + b->Collider.w &&
			a->Collider.x + a->Collider.w > b->Collider.x &&
			a->Collider.y < b->Collider.y + b->Collider.h &&
			a->Collider.y + a->Collider.h > b->Collider.y);
	}

	bool CheckCircleCircleCollision(ColliderComponent* a, ColliderComponent* b) {
		float dx = a->CircleCollider.x - b->CircleCollider.x;
		float dy = a->CircleCollider.y - b->CircleCollider.y;
		float distanceSquared = dx * dx + dy * dy;
		float radiusSum = a->CircleRadius + b->CircleRadius;
		return distanceSquared <= radiusSum * radiusSum;
	}

	
	bool CheckRectCircleCollision(ColliderComponent* rect, ColliderComponent* circle) {
		float closestX = std::clamp(circle->CircleCollider.x, static_cast<float>(rect->Collider.x), static_cast<float>(rect->Collider.x + rect->Collider.w));
		float closestY = std::clamp(circle->CircleCollider.y, static_cast<float>(rect->Collider.y), static_cast<float>(rect->Collider.y + rect->Collider.h));
		float dx = circle->CircleCollider.x - closestX;
		float dy = circle->CircleCollider.y - closestY;
		return (dx * dx + dy * dy) <= circle->CircleRadius * circle->CircleRadius;
	}

	bool CheckCollision(GameObject* objA, GameObject* objB) {
		auto colliderA = objA->GetComponent<ColliderComponent>();
		auto colliderB = objB->GetComponent<ColliderComponent>();

		if (!colliderA || !colliderB) return false;

		if (colliderA->Type == ColliderComponent::ColliderType::RECTANGLE && colliderB->Type == ColliderComponent::ColliderType::RECTANGLE)
			return CheckRectRectCollision(colliderA.get(), colliderB.get());
		if (colliderA->Type == ColliderComponent::ColliderType::RECTANGLE && colliderB->Type == ColliderComponent::ColliderType::CIRCLE)
			return CheckRectCircleCollision(colliderA.get(), colliderB.get());
		if (colliderA->Type == ColliderComponent::ColliderType::CIRCLE && colliderB->Type == ColliderComponent::ColliderType::RECTANGLE)
			return CheckRectCircleCollision(colliderB.get(), colliderA.get());
		if (colliderA->Type == ColliderComponent::ColliderType::CIRCLE && colliderB->Type == ColliderComponent::ColliderType::CIRCLE)
			return CheckCircleCircleCollision(colliderA.get(), colliderB.get());

		return false;
	}
#pragma endregion

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

	// Разрешение коллизии круг-прямоугольник
	void ResolveCircleRectCollision(GameObject* circleObj, GameObject* rectObj) {
		ResolveRectCircleCollision(rectObj, circleObj); // Симметричный случай
	}

	// Разрешение коллизии прямоугольник-прямоугольник с учетом статических объектов
	void ResolveRectRectCollision(GameObject* objA, GameObject* objB) {
		auto transformA = objA->GetComponent<TransformComponent>();
		auto colliderA = objA->GetComponent<ColliderComponent>();
		auto physicsA = objA->GetComponent<PhysicsComponent>();

		auto transformB = objB->GetComponent<TransformComponent>();
		auto colliderB = objB->GetComponent<ColliderComponent>();
		auto physicsB = objB->GetComponent<PhysicsComponent>();

		if (!transformA || !colliderA || !physicsA || !transformB || !colliderB || !physicsB) return;

		// Вычисляем перекрытие по осям X и Y
		int overlapX = std::min(
			colliderA->Collider.x + colliderA->Collider.w - colliderB->Collider.x,
			colliderB->Collider.x + colliderB->Collider.w - colliderA->Collider.x
		);

		int overlapY = std::min(
			colliderA->Collider.y + colliderA->Collider.h - colliderB->Collider.y,
			colliderB->Collider.y + colliderB->Collider.h - colliderA->Collider.y
		);

		if (overlapX < overlapY) {
			// Перекрытие больше по оси X
			if (colliderA->Collider.x < colliderB->Collider.x) {
				if (!physicsA->IsStatic) transformA->Position.x -= overlapX;
				if (!physicsB->IsStatic) transformB->Position.x += overlapX;
			}
			else {
				if (!physicsA->IsStatic) transformA->Position.x += overlapX;
				if (!physicsB->IsStatic) transformB->Position.x -= overlapX;
			}
		}
		else {
			// Перекрытие больше по оси Y
			if (colliderA->Collider.y < colliderB->Collider.y) {
				if (!physicsA->IsStatic) transformA->Position.y -= overlapY;
				if (!physicsB->IsStatic) transformB->Position.y += overlapY;
			}
			else {
				if (!physicsA->IsStatic) transformA->Position.y += overlapY;
				if (!physicsB->IsStatic) transformB->Position.y -= overlapY;
			}
		}

		// Обновляем позиции коллайдеров
		colliderA->UpdatePosition(transformA->Position);
		colliderB->UpdatePosition(transformB->Position);
	}

	// Разрешение коллизии прямоугольник-круг с учетом статических объектов
	void ResolveRectCircleCollision(GameObject* rectObj, GameObject* circleObj) {
		auto transformRect = rectObj->GetComponent<TransformComponent>();
		auto colliderRect = rectObj->GetComponent<ColliderComponent>();
		auto physicsRect = rectObj->GetComponent<PhysicsComponent>();

		auto transformCircle = circleObj->GetComponent<TransformComponent>();
		auto colliderCircle = circleObj->GetComponent<ColliderComponent>();
		auto physicsCircle = circleObj->GetComponent<PhysicsComponent>();

		if (!transformRect || !colliderRect || !physicsRect || !transformCircle || !colliderCircle || !physicsCircle) return;

		// Находим ближайшую точку на прямоугольнике к центру круга
		float closestX = std::clamp(
			colliderCircle->CircleCollider.x,
			static_cast<float>(colliderRect->Collider.x),
			static_cast<float>(colliderRect->Collider.x + colliderRect->Collider.w)
		);
		float closestY = std::clamp(
			colliderCircle->CircleCollider.y,
			static_cast<float>(colliderRect->Collider.y),
			static_cast<float>(colliderRect->Collider.y + colliderRect->Collider.h)
		);

		// Вычисляем вектор между центром круга и ближайшей точкой
		float dx = colliderCircle->CircleCollider.x - closestX;
		float dy = colliderCircle->CircleCollider.y - closestY;

		if (dx == 0 && dy == 0) return; // Круг полностью внутри прямоугольника

		// Вычисляем расстояние до ближайшей точки
		float distance = std::sqrt(dx * dx + dy * dy);
		float overlap = colliderCircle->CircleRadius - distance;

		if (overlap > 0) {
			// Нормализуем вектор направления
			float nx = dx / distance;
			float ny = dy / distance;

			// Определяем, как двигать объекты
			if (!physicsCircle->IsStatic && !physicsRect->IsStatic) {
				// Оба объекта динамические, двигаем их на равные расстояния
				transformCircle->Position.x += nx * overlap / 2;
				transformCircle->Position.y += ny * overlap / 2;

				transformRect->Position.x -= nx * overlap / 2;
				transformRect->Position.y -= ny * overlap / 2;
			}
			else if (!physicsCircle->IsStatic && physicsRect->IsStatic) {
				// Только круг динамический, двигаем только его
				transformCircle->Position.x += nx * overlap;
				transformCircle->Position.y += ny * overlap;
			}
			else if (physicsCircle->IsStatic && !physicsRect->IsStatic) {
				// Только прямоугольник динамический, двигаем только его
				transformRect->Position.x -= nx * overlap;
				transformRect->Position.y -= ny * overlap;
			}

			// Обновляем позиции коллайдеров
			colliderCircle->UpdatePosition(transformCircle->Position);
			colliderRect->UpdatePosition(transformRect->Position);
		}
	}

	// Разрешение коллизии круг-круг с учетом статических объектов
	void ResolveCircleCircleCollision(GameObject* circleA, GameObject* circleB) {
		auto transformA = circleA->GetComponent<TransformComponent>();
		auto colliderA = circleA->GetComponent<ColliderComponent>();
		auto physicsA = circleA->GetComponent<PhysicsComponent>();

		auto transformB = circleB->GetComponent<TransformComponent>();
		auto colliderB = circleB->GetComponent<ColliderComponent>();
		auto physicsB = circleB->GetComponent<PhysicsComponent>();

		if (!transformA || !colliderA || !physicsA || !transformB || !colliderB || !physicsB) return;

		// Вычисляем вектор между центрами двух кругов
		float dx = colliderB->CircleCollider.x - colliderA->CircleCollider.x;
		float dy = colliderB->CircleCollider.y - colliderA->CircleCollider.y;

		// Вычисляем расстояние между центрами
		float distance = std::sqrt(dx * dx + dy * dy);
		if (distance == 0) return; // Центры совпадают, ничего не делаем

		// Сумма радиусов для определения коллизии
		float radiusSum = colliderA->CircleRadius + colliderB->CircleRadius;

		// Проверяем, есть ли пересечение
		if (distance < radiusSum) {
			float overlap = radiusSum - distance; // Размер пересечения

			// Нормализуем вектор между центрами
			float nx = dx / distance;
			float ny = dy / distance;

			// Определяем, как двигать объекты
			if (!physicsA->IsStatic && !physicsB->IsStatic) {
				// Оба объекта динамические, двигаем их на равные расстояния
				transformA->Position.x -= nx * overlap / 2;
				transformA->Position.y -= ny * overlap / 2;

				transformB->Position.x += nx * overlap / 2;
				transformB->Position.y += ny * overlap / 2;
			}
			else if (!physicsA->IsStatic && physicsB->IsStatic) {
				// Только первый объект динамический, двигаем только его
				transformA->Position.x -= nx * overlap;
				transformA->Position.y -= ny * overlap;
			}
			else if (physicsA->IsStatic && !physicsB->IsStatic) {
				// Только второй объект динамический, двигаем только его
				transformB->Position.x += nx * overlap;
				transformB->Position.y += ny * overlap;
			}

			// Обновляем позиции коллайдеров
			colliderA->UpdatePosition(transformA->Position);
			colliderB->UpdatePosition(transformB->Position);
		}
	}
};