#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/StateComponent.h"

#include "Camera.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <SDL.h>
#include "Component.h"

class GameObject {
public:
    std::unordered_map<std::string, std::shared_ptr<Component>> components;

    template <typename T>
    void AddComponent(std::shared_ptr<T> component) {
        components[typeid(T).name()] = component;
    }

    template <typename T>
    std::shared_ptr<T> GetComponent() {
        return std::static_pointer_cast<T>(components[typeid(T).name()]);
    }

    virtual void Update(float deltaTime);
    virtual void Draw(SDL_Renderer* renderer, const Camera& camera);

    void ResolveCollision(GameObject* other);
    bool CheckCollision(GameObject* other);

private:
    void ApplyRepulsion(GameObject* obj, GameObject* other);
};

#endif // GAME_OBJECT_H