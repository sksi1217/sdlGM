﻿#pragma once
#include "../Core/Component.h"
#include <SDL.h>

class ColliderComponent : public Component {
public:
    enum class ColliderType { RECTANGLE, CIRCLE };

    enum class Layer : int {
        None = 0,          // Без коллайдера
        Player = 1,        // Игрок
        Enemy = 2,         // Враги
        Bullet = 3,        // Пули
        Wall = 4,          // Стены
        Item = 5,          // Предметы (зелья, монеты)
        Trap = 6,          // Ловушки
        Door = 7,          // Двери
        Boss = 8,          // Боссы
        Particle = 9,      // Частицы (эффекты)
        Trigger = 10       // Триггеры (например, зона спавна)
    };

    SDL_Rect Collider = { 0, 0, 1, 1 }; // Коллизия для прямоугольника
    SDL_FPoint CircleCollider = { 0.0f, 0.0f }; // Коллизия для круга (центр)
    float CircleRadius = 1.0f; // Радиус круга

    int OffsetColliderX = 0;          // Смещение коллизии по X
    int OffsetColliderY = 0;          // Смещение коллизии по Y
    int WidthColliderX = 16;           // Ширина коллизии (для прямоугольника)
    int HeightColliderY = 16;          // Высота коллизии (для прямоугольника)

    ColliderType Type = ColliderType::RECTANGLE; // Тип коллайдера (по умолчанию прямоугольник)
    Layer m_layer = Layer::Player; // Тип коллайдера (по умолчанию прямоугольник)

    // Установка типа коллайдера
    void SetColliderType(ColliderType type) { Type = type; }
    void SetLayer(Layer layer) { m_layer = layer; }

    // Обновление позиции коллайдера
    void UpdatePosition(const SDL_FPoint& position) {
        if (Type == ColliderType::RECTANGLE) {
            Collider.x = static_cast<int>(position.x + OffsetColliderX);
            Collider.y = static_cast<int>(position.y + OffsetColliderY);
            Collider.w = WidthColliderX;
            Collider.h = HeightColliderY;
        }
        else if (Type == ColliderType::CIRCLE) {
            CircleCollider.x = position.x + OffsetColliderX;
            CircleCollider.y = position.y + OffsetColliderY;
        }
    }


    

};