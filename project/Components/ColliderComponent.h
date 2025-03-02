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
        Collider.x = position.x + OffsetColliderX;
        Collider.y = position.y + OffsetColliderY;
        Collider.w = WidthColliderX;
        Collider.h = HeightColliderY;
    }

    


    
};