#pragma once
#include <vector>
#include <memory>
#include "../Core/GameObject.h"

class CollisionSystem {
public:
    void Update(std::vector<std::shared_ptr<GameObject>>& entities);
};