#pragma once
#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "../Core/Component.h"
#include <memory>

class Animation; // Forward declaration

class AnimationComponent : public Component {
public:
    int SpriteRow = 0;

    std::shared_ptr<Animation> animation = nullptr; // Указатель на анимацию
};

#endif // ANIMATION_COMPONENT_H