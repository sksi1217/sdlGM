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

    float GetAccuracy() const { return m_accuracy; }
    void SetAccuracy(float accuracy) { m_accuracy = accuracy; }

    float GetDodgeChance() const { return m_dodgeChance; }
    void SetDodgeChance(float dodgeChance) { m_dodgeChance = dodgeChance; }

    float GetArmorPenetration() const { return m_armorPenetration; }
    void SetArmorPenetration(float armorPenetration) { m_armorPenetration = armorPenetration; }

    float GetCooldownReduction() const { return m_cooldownReduction; }
    void SetCooldownReduction(float CooldownReduction) { m_cooldownReduction = CooldownReduction; }

private:
    int m_strength = 10; // Сила
    int m_agility = 10;  // Ловкость
    int m_intelligence = 10; // Интеллект

    int m_armor = 0; // Броня
    float m_dodgeChance = 0.1f; // Шанс уклонения

    float m_cooldownReduction = 0; // Времени перезарядки способностей/оружия

    float m_accuracy = 0.2; // Точность (вероятность попадания)
    float m_armorPenetration = 0.0f; // Проникновение брони
};