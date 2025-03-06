#pragma once

#include "../Core/Component.h"
#include <unordered_map>
#include <string>

class AbilityComponent : public Component {
public:
    // Добавление способности
    void AddAbility(const std::string& abilityName, float cooldown, float duration) {
        m_abilities[abilityName] = { cooldown, duration, 0.0f };
    }

    // Использование способности
    bool UseAbility(const std::string& abilityName, float deltaTime) {
        auto it = m_abilities.find(abilityName);
        if (it != m_abilities.end()) {
            AbilityInfo& info = it->second;
            if (info.cooldownRemaining <= 0.0f) {
                info.cooldownRemaining = info.cooldown;
                info.durationRemaining = info.duration;
                return true; // Способность успешно использована
            }
        }
        return false; // Способность ещё на кулдауне
    }

    // Обновление состояния способностей
    void UpdateAbilities(float deltaTime) {
        for (auto& [name, info] : m_abilities) {
            if (info.cooldownRemaining > 0.0f) {
                info.cooldownRemaining -= deltaTime;
            }
            if (info.durationRemaining > 0.0f) {
                info.durationRemaining -= deltaTime;
            }
        }
    }

private:
    struct AbilityInfo {
        float cooldown; // Время кулдауна
        float duration; // Длительность эффекта
        float cooldownRemaining = 0.0f; // Оставшееся время кулдауна
        float durationRemaining = 0.0f; // Оставшееся время действия
    };

    std::unordered_map<std::string, AbilityInfo> m_abilities;
};