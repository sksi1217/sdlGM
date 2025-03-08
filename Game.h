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
        float gameTime = 0.0f;
        int currentPhase = 0;
        float timeSinceLastWave = 0.0f;

        struct PhaseConfig {
            int phase;
            float minTime;      // Время начала фазы
            int commonEnemies;  // Количество обычных врагов в волне
            int heavyEnemies;   // Количество тяжелых врагов
            bool spawnBoss;     // Спавнить ли босса в этой фазе
            float waveInterval; // Интервал между волнами
        };

        // Таблица фаз (настройте под себя!)
        std::vector<PhaseConfig> phases = {
            {0, 0.0f,   3, 0, false, 5.0f},   // Фаза 0: Только обычные враги
            {1, 30.0f,  4, 1, false, 4.0f},   // Фаза 1: Добавлены тяжелые враги
            {2, 60.0f,  5, 2, true,  3.0f}    // Фаза 2: Босс каждые 3 секунды
        };

        void Update(float deltaTime) {
            gameTime += deltaTime;
            timeSinceLastWave += deltaTime;

            // Определение текущей фазы
            for (const auto& phase : phases) {
                if (gameTime >= phase.minTime) {
                    currentPhase = phase.phase;
                }
            }
        }

        bool shouldSpawnWave() const {
            return timeSinceLastWave >= phases[currentPhase].waveInterval;
        }

        void resetTimer() {
            timeSinceLastWave = 0.0f;
        }

        const PhaseConfig& getCurrentPhase() const {
            return phases[currentPhase];
        }
    };


    PacingSystem pacingSystem;
    void spawnWave();
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