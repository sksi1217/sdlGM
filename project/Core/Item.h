#pragma once


class Item
{
public:
    int m_damage = 10; // Базовый урон (оружия)
    float m_criticalChance = 0.1f; // Шанс критического удара (0 = 0%, 1 = 100%) (оружия)
    // Перезарядка
    float m_fireRate = 5; // Интервал между залпами (в секундах)
    float m_bulletInterval = 0.5; // Интервал между пулями внутри залпа (в секундах)
    float m_range = 100.0f; // Дальность атаки (оружия)
    int m_shells = 1; // Кол-во снарядов
    float accuracy = 0.5; // точность

    float m_vampirism = 0.0f; // Вамперизм

    float m_lifetimeBullet = 1; // Жизнь пули 

    float m_movementSpeedModifier = 1.0f; // Модификатор скорости
};