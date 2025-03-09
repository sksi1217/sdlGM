#pragma once

#include "../Core/Component.h"

class AttributesComponent : public Component {
public:
    int m_armor = 0; // Броня (игрок)
    float m_dodgeChance = 0; // Шанс уклонения (игрок)
    float m_vampirism = 10;
};