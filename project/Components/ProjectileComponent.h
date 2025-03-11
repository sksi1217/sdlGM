// ProjectileComponent.h
#pragma once
#include "../Components/EffectComponent.h"
#include <SDL.h>

struct ProjectileComponent : public Component {
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

    enum class Type {
        Straight,
        ZigZag,
        Circular,
        Returning,
        Spiral,
        Ricochet,
        Wave,
        Chaotic,
        HyperAcceleration,
        Orbital,
        Impaling,
        Splitter,
        Slowing,
        Singular
    };

    // Список активных типов движения
    std::vector<Type> types;

    // Флаг для проверки наличия позиционных типов
    bool HasPositionalType() const {
        for (auto type : types) {
            switch (type) {
            case Type::Circular:
            case Type::Orbital:
            case Type::Spiral:
            case Type::Returning:
                return true;
            default:
                break;
            }
        }
        return false;
    }

    struct ZigZagParams {
        float amplitude = 50.0f;
        float frequency = 2.0f;
    };

    struct CircularParams {
        float radius = 10.0f;
        float angularSpeed = 90.0f; // degrees per second
        SDL_FPoint center;
    };

    struct SpiralParams {
        float startRadius = 10.0f;
        float expansionSpeed = 10.0f;
        float angularSpeed = 180.0f;
        SDL_FPoint center;
    };

    struct WaveParams {
        float amplitude = 50.0f;
        float frequency = 1.0f;
        bool horizontal = false;
    };

    struct ChaoticParams {
        float changeInterval = 0.5f;
        float maxDeviation = 45.0f;
        float timer = 0.0f;
        SDL_FPoint direction;
    };

    struct HyperAccelerationParams {
        float acceleration = 100.0f;
    };

    struct OrbitalParams {
        float radius = 10.0f;
        float angularSpeed = 180.0f;
        SDL_FPoint center;
    };

    struct ReturningParams {
        float returnSpeed = 150.0f;
        float returnDelay = 0.5f;
        float timer = 0.0f;
        bool isReturning = false;
    };

    struct RicochetParams {
        int maxBounces = 3;
        int currentBounces = 0;
        float bounceFactor = 0.8f;
    };

    struct ImpalingParams {
        float pierceChance = 0.3f;
        int maxPierces = 2;
        int pierces = 0;
    };

    struct SplitterParams {
        int fragments = 3;
        float fragmentSpread = 30.0f;
        bool splitOnCollision = true;
    };

    struct SlowingParams {
        float slowAmount = 0.5f;
        float slowDuration = 1.0f;
        float trailInterval = 0.2f;
        float trailTimer = 0.0f;
    };

    struct SingularParams {
        float pullStrength = 2000.0f;
        float radius = 50;
    };

    struct ModificationParams {
        bool rotateAroundCenter = false; // Флаг для вращения вокруг центра
        float rotationSpeed = 200.0f;     // Скорость вращения (градусы в секунду)
        float offsetAmplitude = 10.0f;   // Амплитуда смещения
        float offsetFrequency = 2.0f;    // Частота смещения
    };

    struct RotationAroundTrajectoryParams {
        float radius = 180.0f;       // Радиус вращения вокруг траектории
        float angularSpeed = 180.0f; // Скорость вращения (градусы в секунду)
        float phaseOffset = 0.0f;   // Начальная фаза вращения
    };

    struct RotationParams {
        float radius = 20.0f;       // Радиус вращения
        float angularSpeed = 180.0f; // Скорость вращения (градусы/сек)
        float phaseOffset = 0.0f;   // Начальная фаза
    } rotation;

    SDL_FPoint basePosition; // Базовая позиция пули (без учета вращения)

    Type type = Type::Straight;
    bool isActive = true;
    float lifetime = 5.0f;
    float elapsedTime = 0.0f;

    // Parameters for each type
    ZigZagParams zigzag;
    CircularParams circular;
    SpiralParams spiral;
    WaveParams wave;
    ChaoticParams chaotic;
    HyperAccelerationParams hyperAccel;
    OrbitalParams orbital;
    ReturningParams returning;
    RicochetParams ricochet;
    ImpalingParams impaling;
    SplitterParams splitter;
    SlowingParams slowing;
    SingularParams singular;
    RotationParams rotationParams;

    ModificationParams modificationParams;
    RotationAroundTrajectoryParams rotationAroundTrajectory;

    GameObject* owner;

    int m_damage = 0; // Базовый урон
    float m_criticalChance = 0.0f; // Шанс критического удара (0 = 0%, 1 = 100%)
    float m_criticalMultiplier = 2.0f; // Множитель критического удара
    float m_lifetimeBullet = 1.0f; // Время жизни пули
    float m_vampirism = 0.0f;
    bool m_remove_bullet = true;
};