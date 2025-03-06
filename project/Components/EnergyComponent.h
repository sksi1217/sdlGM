#pragma once

#include "../Core/Component.h"

class EnergyComponent : public Component {
public:
    // Геттеры и сеттеры для энергии/маны

    float GetEnergy() const { return m_energy; }
    void SetEnergy(float energy) { m_energy = energy; }

    float GetMaxEnergy() const { return m_maxEnergy; }
    void SetMaxEnergy(float maxEnergy) { m_maxEnergy = maxEnergy; }

    float GetEnergyRegeneration() const { return m_energyRegeneration; }
    void SetEnergyRegeneration(float energyRegeneration) { m_energyRegeneration = energyRegeneration; }

    // Регенерация энергии
    void RegenerateEnergy(float deltaTime) {
        if (m_energy < m_maxEnergy) {
            m_energy += m_energyRegeneration * deltaTime;
            if (m_energy > m_maxEnergy) {
                m_energy = m_maxEnergy;
            }
        }
    }

private:
    float m_energy = 100.0f; // Текущая энергия/мана
    float m_maxEnergy = 100.0f; // Максимальная энергия/мана
    float m_energyRegeneration = 5.0f; // Регенерация энергии в секунду
};