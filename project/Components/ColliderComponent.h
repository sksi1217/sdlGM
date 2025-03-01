#pragma once
#include "../Core/Component.h"
#include <SDL.h>

class ColliderComponent : public Component {
public:
    SDL_Rect Collider = { 0, 0, 1, 1 }; // Коллизия
    int OffsetColliderX = 0;          // Смещение коллизии по X
    int OffsetColliderY = 0;          // Смещение коллизии по Y
    int WidthColliderX = 0;           // Ширина коллизии
    int HeightColliderY = 0;          // Высота коллизии

    // Обновление позиции коллайдера
    void UpdatePosition(const SDL_FPoint& position) {
        Collider.x = static_cast<int>(position.x + OffsetColliderX);
        Collider.y = static_cast<int>(position.y + OffsetColliderY);
        Collider.w = WidthColliderX;
        Collider.h = HeightColliderY;
    }

    // Проверка столкновения с другим коллайдером
    bool CheckCollision(const ColliderComponent& other) const {
        const SDL_Rect& rectA = Collider;
        const SDL_Rect& rectB = other.Collider;

        return !(
            rectA.x + rectA.w <= rectB.x ||
            rectB.x + rectB.w <= rectA.x ||
            rectA.y + rectA.h <= rectB.y ||
            rectB.y + rectB.h <= rectA.y);
    }

    SDL_FPoint GetIntersectionDepth(const ColliderComponent& other) const {
        const SDL_Rect& rectA = Collider;
        const SDL_Rect& rectB = other.Collider;

        // Вычисляем глубину пересечения по осям X и Y
        float halfWidthA = rectA.w / 2.0f;
        float halfHeightA = rectA.h / 2.0f;
        float halfWidthB = rectB.w / 2.0f;
        float halfHeightB = rectB.h / 2.0f;

        SDL_FPoint centerA = { rectA.x + halfWidthA, rectA.y + halfHeightA };
        SDL_FPoint centerB = { rectB.x + halfWidthB, rectB.y + halfHeightB };

        float distanceX = centerA.x - centerB.x;
        float distanceY = centerA.y - centerB.y;

        float overlapX = rectA.w / 2 + rectB.w / 2 - abs(distanceX);
        float overlapY = rectA.h / 2 + rectB.h / 2 - abs(distanceY);

        if (overlapX > 0 && overlapY > 0) {
            if (overlapX < overlapY) {
                return { overlapX * (distanceX > 0 ? 1 : -1), 0 }; // Пересечение по оси X
            }
            else {
                return { 0, overlapY * (distanceY > 0 ? 1 : -1) }; // Пересечение по оси Y
            }
        }

        return { 0, 0 }; // Нет пересечения
    }
};