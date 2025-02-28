#include "TextureLoader.h"
#include <iostream>

// Статическая переменная для экземпляра TextureLoader
TextureLoader& TextureLoader::GetInstance() {
    static TextureLoader instance;
    return instance;
}

// Конструктор
TextureLoader::~TextureLoader() {
    Clear();
}

// Загрузка текстуры
SDL_Texture* TextureLoader::LoadTexture(const std::string& filePath, SDL_Renderer* renderer) {
    if (textureMap.find(filePath) != textureMap.end()) {
        return textureMap[filePath]; // Возвращаем уже загруженную текстуру
    }

    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << filePath << ". Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);

    if (texture) {
        textureMap[filePath] = texture; // Сохраняем текстуру в мапе
    }

    return texture;
}

// Получение текстуры
SDL_Texture* TextureLoader::GetTexture(const std::string& filePath) {
    return textureMap[filePath];
}

// Очистка текстур
void TextureLoader::Clear() {
    for (auto& pair : textureMap) {
        SDL_DestroyTexture(pair.second);
    }
    textureMap.clear();
}