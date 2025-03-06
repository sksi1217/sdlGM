#pragma once

#include "../Core/Component.h"

class CombatComponent : public Component {
public:
    // Геттеры и сеттеры для боевых характеристик

    float GetAccuracy() const { return m_accuracy; }
    void SetAccuracy(float accuracy) { m_accuracy = accuracy; }

    float GetDodgeChance() const { return m_dodgeChance; }
    void SetDodgeChance(float dodgeChance) { m_dodgeChance = dodgeChance; }

    float GetArmorPenetration() const { return m_armorPenetration; }
    void SetArmorPenetration(float armorPenetration) { m_armorPenetration = armorPenetration; }

    float GetDamageResistance() const { return m_damageResistance; }
    void SetDamageResistance(float damageResistance) { m_damageResistance = damageResistance; }

private:
    float m_accuracy = 0.9f; // Точность (вероятность попадания)
    float m_dodgeChance = 0.1f; // Шанс уклонения
    float m_armorPenetration = 0.0f; // Проникновение брони
    float m_damageResistance = 0.0f; // Сопротивление урону
};