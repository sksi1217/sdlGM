#pragma once

#include "../Core/Component.h"

class ResourceGatheringComponent : public Component {
public:
    // Геттеры и сеттеры для параметров сбора ресурсов

    float GetGatherRadius() const { return m_gatherRadius; }
    void SetGatherRadius(float gatherRadius) { m_gatherRadius = gatherRadius; }

    float GetSucklingRadius() const { return m_sucklingRadius; }
    void SetSucklingRadius(float sucklingRadius) { m_sucklingRadius = sucklingRadius; }

    // Проверка расстояния между двумя точками
    float IsWithinRange(const SDL_FPoint& position1, const SDL_FPoint& position2) const {
        float dx = position1.x - position2.x;
        float dy = position1.y - position2.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        return distance;
    }

private:
    float m_sucklingRadius = 20.0f; // Радиус присасывания маны
    float m_gatherRadius = 5.0f; // Радиус сбора ресурсов
};