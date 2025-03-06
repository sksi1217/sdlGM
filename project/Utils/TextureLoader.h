#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

class TextureLoader {
public:
    static TextureLoader& GetInstance();

    // Установка базового пути к ресурсам
    void SetBasePath(const std::string& path) {
        basePath = path;
    }

    // Загрузка текстуры по относительному пути
    SDL_Texture* LoadTexture(const std::string& relativePath, SDL_Renderer* renderer);

    // Получение текстуры по относительному пути
    SDL_Texture* GetTexture(const std::string& relativePath);

    void Clear();

private:
    TextureLoader() = default;
    ~TextureLoader();

    std::unordered_map<std::string, SDL_Texture*> textureMap;
    std::string basePath;

    TextureLoader(const TextureLoader&) = delete;
    TextureLoader& operator=(const TextureLoader&) = delete;
};