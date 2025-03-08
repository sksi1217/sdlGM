#pragma once
#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "project/Resources/ManagerGame.h"

class Game {
public:
    // Designer
    Game(const char* title, int width, int height);

    // Destructor
    ~Game();

    // Main Game Cycle
    void Run();


    

private:
    struct PacingSystem {
        float timeSinceLastWave = 0.0f;
        float waveInterval = 5.0f; // Начальный интервал между волнами
        int enemiesPerWave = 3;    // Начальное количество врагов
        float gameTime = 0.0f;

        void Update(float deltaTime) {
            gameTime += deltaTime;
            timeSinceLastWave += deltaTime;

            // Пример нарастающей сложности
            waveInterval = std::max(1.0f, 5.0f - gameTime * 0.1f);
            enemiesPerWave = 3 + static_cast<int>(gameTime * 0.5f);
        }

        bool shouldSpawnWave() const {
            return timeSinceLastWave >= waveInterval;
        }

        void resetTimer() {
            timeSinceLastWave = 0.0f;
        }
    };

    PacingSystem pacingSystem;
    void spawnWave(int count);
    SDL_Rect generateSpawnPoint() const;

    bool isRunning = true;
    SDL_Window* window = nullptr;
    // SDL_Renderer* renderer = nullptr;

    // Initialization
    void Initialize();

    // Resource Loading
    void LoadContent();

    // Logic Update
    void Update(float deltaTime);

    // Frame rendering
    void Draw();

    // Resource offloading
    void UnloadContent();

    // Event Handling
    void HandleEvents();
};

#endif // GAME