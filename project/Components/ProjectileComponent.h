#pragma once

#include "../Utils/MathUtils.h"
#include "../Core/Component.h"
#include <SDL.h>
#include <utility>

class ProjectileComponent : public Component {
public:

    // Метод для расчета урона с учётом характеристик цели
    float CalculateDamage(float targetArmor, float targetEvasion) {
        // 1. Проверка уклонения
        if (MathUtils::NextFloat() <= targetEvasion) {
            return 0.0f; // Цель уклонилась
        }

        // 2. Базовый урон с критическим ударом
        float totalDamage = m_damage;
        if (IsCriticalHit()) {
            totalDamage *= m_criticalMultiplier;
        }

        // 3. Применение брони (пример формулы: снижение урона на процент)
        float armorReduction = targetArmor / (targetArmor + 100.0f); // Формула брони
        totalDamage *= (1.0f - armorReduction);

        // Гарантируем, что урон не отрицательный
        return std::max(totalDamage, 0.0f);
    }

    // Проверка критического удара
    bool IsCriticalHit() {
        return MathUtils::NextFloat() < m_criticalChance;
    }


    bool m_remove_bullet = true;

    // Боевые характеристики
    int m_damage = 0; // Базовый урон
    float m_criticalChance = 0.0f; // Шанс критического удара (0 = 0%, 1 = 100%)
    float m_lifetimeBullet = 1.0f; // Время жизни пули
    float m_vampirism = 0.0f;
    
    // 
    float m_range = 20.0f;
    

    // Не меняется для оружия
    float m_orbitAngleDegrees = 0.0f;  // Текущий угол в градусах
    GameObject* owner;
    TransformComponent* m_playerTransform = nullptr;
    float m_criticalMultiplier = 2.0f; // Множитель критического удара
    float m_elapsedTime = 0.0f; // Время, прошедшее с последнего выстрела
};