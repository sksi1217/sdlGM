#include "Player.h"
#include <iostream>

// Инициальзация
Player::Player(const SDL_FPoint& startPosition, SDL_Texture* texture) {
    // TransformComponent: Начальная позиция и масштаб
    auto transform = std::make_shared<TransformComponent>();
    transform->Position = startPosition;
    AddComponent(transform);

    // MovementComponent: Скорость движения
    auto movement = std::make_shared<MovementComponent>();
    AddComponent(movement);

    // StateComponent: Активность и коллизии
    auto state = std::make_shared<StateComponent>();
    AddComponent(state);

    // PhysicsComponent: Масса и сила отталкивания
    auto physics = std::make_shared<PhysicsComponent>();
    AddComponent(physics);

    // RenderComponent: Текстура и цвет
    auto render = std::make_shared<RenderComponent>();
    render->Texture = texture;
    if (!render->Texture) std::cerr << "Failed to load player texture: " << texture << std::endl;
    AddComponent(render);

    // AnimationComponent: Настройка анимации
    auto animation = std::make_shared<AnimationComponent>();
    animation->animation = std::make_shared<Animation>(16, 16, 8, 1.0f / (movement->Speed * 0.2f));
    AddComponent(animation);
}

void Player::Update(float deltaTime) {
    auto transform = GetComponent<TransformComponent>();
    auto movement = GetComponent<MovementComponent>();

    if (!transform || !movement) return;

    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    SDL_FPoint velocity = { 0.0f, 0.0f };

    HandleMovement(keyboardState, velocity);

    transform->Position.x += velocity.x * movement->Speed * deltaTime;
    transform->Position.y += velocity.y * movement->Speed * deltaTime;

    std::cout << transform->Position.x << std::endl;
    std::cout << transform->Position.y << std::endl;

    // Обновление анимации (если есть)
    auto animationComponent = GetComponent<AnimationComponent>();
    if (animationComponent && animationComponent->animation) {
        bool isMoving = false;
        if (keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_S] ||
            keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_D]) {
            isMoving = true;
        }
        animationComponent->animation->Update(isMoving, static_cast<Uint32>(deltaTime * 1000.0f));
    }
}

// Обработка движения
void Player::HandleMovement(const Uint8* keyboardState, SDL_FPoint& velocity) {
    if (keyboardState[SDL_SCANCODE_W]) velocity.y -= 1.0f; // Вверх
    if (keyboardState[SDL_SCANCODE_S]) velocity.y += 1.0f; // Вниз
    if (keyboardState[SDL_SCANCODE_A]) velocity.x -= 1.0f; // Влево
    if (keyboardState[SDL_SCANCODE_D]) velocity.x += 1.0f; // Вправо

    if (velocity.x != 0.0f || velocity.y != 0.0f) {
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity.x /= length;
        velocity.y /= length;

        UpdateSpriteRow(velocity);   
    }
}

// Обновление спрайтов
void Player::UpdateSpriteRow(const SDL_FPoint& velocity) {
    auto animation = GetComponent<AnimationComponent>();

    if (std::abs(velocity.x) > std::abs(velocity.y)) {
        animation->SpriteRow = (velocity.x > 0.0f) ? RightRow : LeftRow;
    }
    else {
        animation->SpriteRow = (velocity.y > 0.0f) ? DownRow : UpRow;
    }
}