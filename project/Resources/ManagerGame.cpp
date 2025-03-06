#include "ManagerGame.h"

// Определение статического члена
SDL_Renderer* ManagerGame::renderer = nullptr;
std::vector<std::shared_ptr<GameObject>> ManagerGame::objects;
std::vector<std::shared_ptr<GameObject>> ManagerGame::enemies;
std::vector<std::shared_ptr<GameObject>> ManagerGame::experience;
std::vector<std::shared_ptr<GameObject>> ManagerGame::_allWeapons;