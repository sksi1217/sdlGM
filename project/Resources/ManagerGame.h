﻿#pragma once
#ifndef MANAGERGAME_H
#define MANAGERGAME_H
#include "../Core/GameObject.h"
#include <vector>
#include <memory>

class ManagerGame {
public:
    // Создание списка объектов 
    static std::vector<std::shared_ptr<GameObject>> objects;
};
#endif // MANAGERGAME_H