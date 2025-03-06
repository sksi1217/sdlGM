#include "TextureLoader.h"
#include <iostream>

TextureLoader& TextureLoader::GetInstance() {
    static TextureLoader instance;
    return instance;
}

TextureLoader::~TextureLoader() {
    Clear();
}

SDL_Texture* TextureLoader::LoadTexture(const std::string& relativePath, SDL_Renderer* renderer) {
    // Формируем полный путь
    fs::path fullPath = fs::path(basePath) / relativePath;
    std::string full_path_str = fullPath.string();

    // Проверяем, есть ли текстура уже загружена
    if (textureMap.find(full_path_str) != textureMap.end()) {
        return textureMap[full_path_str];
    }

    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(full_path_str.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << full_path_str << ". Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);

    if (texture) {
        textureMap[full_path_str] = texture;
    }

    return texture;
}

SDL_Texture* TextureLoader::GetTexture(const std::string& relativePath) {
    fs::path fullPath = fs::path(basePath) / relativePath;
    std::string full_path_str = fullPath.string();

    auto it = textureMap.find(full_path_str);
    return it != textureMap.end() ? it->second : nullptr;
}

void TextureLoader::Clear() {
    for (auto& pair : textureMap) {
        SDL_DestroyTexture(pair.second);
    }
    textureMap.clear();
}