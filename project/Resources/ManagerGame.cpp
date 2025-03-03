#include "ManagerGame.h"

// Определение статического члена
std::vector<std::shared_ptr<GameObject>> ManagerGame::objects;
std::vector<std::shared_ptr<GameObject>> ManagerGame::enemies;
std::vector<std::shared_ptr<GameObject>> ManagerGame::_allWeapons;