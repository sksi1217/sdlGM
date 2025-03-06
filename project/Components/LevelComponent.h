#pragma once

#include "../Core/Component.h"

class LevelComponent : public Component {
public:
    // Геттеры и сеттеры для уровня и опыта

    int GetLevel() const { return m_level; }
    void SetLevel(int level) { m_level = level; }

    float GetExperience() const { return m_experience; }
    void SetExperience(float experience) { m_experience = experience; }

    float GetExperienceToNextLevel() const { return m_experienceToNextLevel; }

    // Добавление опыта
    void AddExperience(float experience) {
        m_experience += experience;
        while (m_experience >= m_experienceToNextLevel) {
            m_experience -= m_experienceToNextLevel;
            m_experienceToNextLevel *= 1.5f; // Увеличение требуемого опыта
            ++m_level;
        }
    }

private:
    int m_level = 1; // Текущий уровень
    float m_experience = 0.0f; // Текущий опыт
    float m_experienceToNextLevel = 100.0f; // Опыт для следующего уровня
};