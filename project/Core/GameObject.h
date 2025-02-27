#pragma once
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <unordered_map>
#include <string>
#include <memory>

class Component {};

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
};

#endif // GAME_OBJECT_H