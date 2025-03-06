#pragma once

#include "../Core/Component.h"

class ResourceGatheringComponent : public Component {
public:
    // Геттеры и сеттеры для параметров сбора ресурсов

    float GetGatherRadius() const { return m_gatherRadius; }
    void SetGatherRadius(float gatherRadius) { m_gatherRadius = gatherRadius; }

    // Проверка расстояния между двумя точками
    bool IsWithinRange(const SDL_FPoint& position1, const SDL_FPoint& position2) const {
        float dx = position1.x - position2.x;
        float dy = position1.y - position2.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        return distance <= m_gatherRadius;
    }

private:
    float m_gatherRadius = 5.0f; // Радиус сбора ресурсов
};