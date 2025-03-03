#pragma once
#ifndef MANAGERGAME_H
#define MANAGERGAME_H
#include "../Core/GameObject.h"
#include "../Core/Weapon.h"
#include <vector>
#include <memory>


class ManagerGame {
public:
    static std::vector<std::shared_ptr<GameObject>> _allWeapons;

    // Создание списка объектов 
    static std::vector<std::shared_ptr<GameObject>> objects;
    static std::vector<std::shared_ptr<GameObject>> enemies;
};
#endif // MANAGERGAME_H