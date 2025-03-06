#pragma once

#include "../Core/Component.h"

class AttributesComponent : public Component {
public:
    // Геттеры и сеттеры для атрибутов

    int GetStrength() const { return m_strength; }
    void SetStrength(int strength) { m_strength = strength; }

    int GetAgility() const { return m_agility; }
    void SetAgility(int agility) { m_agility = agility; }

    int GetIntelligence() const { return m_intelligence; }
    void SetIntelligence(int intelligence) { m_intelligence = intelligence; }

    int GetArmor() const { return m_armor; }
    void SetArmor(int armor) { m_armor = armor; }

private:
    int m_strength = 10; // Сила
    int m_agility = 10;  // Ловкость
    int m_intelligence = 10; // Интеллект
    int m_armor = 0; // Броня
};