#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <unordered_map>
#include <memory>

class TextureLoader {
public:
    // Одиночка (singleton) для доступа к TextureLoader
    static TextureLoader& GetInstance();

    // Загрузка текстуры по пути
    SDL_Texture* LoadTexture(const std::string& filePath, SDL_Renderer* renderer);

    // Получение уже загруженной текстуры
    SDL_Texture* GetTexture(const std::string& filePath);

    // Очистка всех загруженных текстур
    void Clear();

private:
    TextureLoader() = default; // Приватный конструктор для одиночки
    ~TextureLoader();          // Приватный деструктор

    // Карта для хранения загруженных текстур
    std::unordered_map<std::string, SDL_Texture*> textureMap;

    // Запрет на копирование и присваивание
    TextureLoader(const TextureLoader&) = delete;
    TextureLoader& operator=(const TextureLoader&) = delete;
};