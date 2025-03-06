#pragma once

#include "../Core/Component.h"
#include <unordered_map>
#include <string>

class EffectComponent : public Component {
public:
    // Применение эффекта
    void ApplyEffect(const std::string& effectName, float value, float duration) {
        m_effects[effectName] = { value, duration };
    }

    // Обновление эффектов
    void UpdateEffects(float deltaTime) {
        for (auto it = m_effects.begin(); it != m_effects.end(); ) {
            EffectInfo& info = it->second;
            info.duration -= deltaTime;
            if (info.duration <= 0.0f) {
                it = m_effects.erase(it); // Удалить истёкший эффект
            }
            else {
                ++it;
            }
        }
    }

    // Проверка наличия эффекта
    bool HasEffect(const std::string& effectName) const {
        return m_effects.find(effectName) != m_effects.end();
    }

private:
    struct EffectInfo {
        float value; // Значение эффекта
        float duration; // Время действия
    };

    std::unordered_map<std::string, EffectInfo> m_effects;
};