#pragma once
#ifndef MANAGERGAME_H
#define MANAGERGAME_H

#include <vector>
#include <memory>
#include "../Core/GameObject.h"
#include <SDL.h>


class ManagerGame {
public:
    static SDL_Renderer* renderer; // Глобальный рендерер
    static std::vector<std::shared_ptr<GameObject>> objects;
    static std::vector<std::shared_ptr<GameObject>> enemies;
    static std::vector<std::shared_ptr<GameObject>> experience;

    static std::vector<std::shared_ptr<GameObject>> _allWeapons;
};
#endif // MANAGERGAME_H