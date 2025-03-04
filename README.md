# sdl2


Алгоритм Брезенхэма использую для рисования круга:
https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm


Elliptic orbit:
https://en.m.wikipedia.org/wiki/Elliptic_orbit

Гомановская траектория:
https://en.wikipedia.org/wiki/Hohmann_transfer_orbit



---

### 2. **Взаимодействия между слоями (Пример)**

#### Матрица коллизий:
| Слой       | Player | Enemy | Bullet | Wall | Item | Trap | Door | Boss | Particle | Trigger |
|------------|--------|-------|--------|------|------|------|------|------|----------|---------|
| **Player**  | -      | Да    | Нет    | Да   | Да   | Да   | Да   | Да   | Нет      | Да      |
| **Enemy**   | Да     | -     | Да     | Да   | Нет  | Да   | Да   | Нет  | Нет      | Нет     |
| **Bullet**  | Нет    | Да    | -      | Да   | Нет  | Нет  | Нет  | Да   | Нет      | Нет     |
| **Wall**    | Да     | Да    | Да     | -    | Нет  | Нет  | Нет  | Нет  | Нет      | Нет     |
| **Item**    | Да     | Нет   | Нет    | Нет  | -    | Нет  | Нет  | Нет  | Нет      | Нет     |
| **Trap**    | Да     | Да    | Нет    | Нет  | Нет  | -    | Нет  | Да   | Нет      | Нет     |
| **Door**    | Да     | Да    | Нет    | Нет  | Нет  | Нет  | -    | Нет  | Нет      | Нет     |
| **Boss**    | Да     | Нет   | Да     | Да   | Нет  | Да   | Нет  | -    | Нет      | Нет     |
| **Particle**| Нет    | Нет   | Нет    | Нет  | Нет  | Нет  | Нет  | Нет  | -        | Нет     |
| **Trigger** | Да     | Нет   | Нет    | Нет  | Нет  | Нет  | Нет  | Нет  | Нет      | -       |

Примерная структкра проекта:
```
project/
├── Components/
│   ├── TransformComponent.h
│   ├── AnimationComponent.h
│   ├── InputComponent.h
│   ├── WeaponComponent.h
│   ├── ColliderComponent.h
│   └── HealthComponent.h
├── Systems/
│   ├── MovementSystem.h
│   ├── AnimationSystem.h
│   ├── WeaponSystem.h
│   ├── CollisionSystem.h
│   ├── AISystem.h
│   └── HealthSystem.h
├── Entities/
│   ├── Player.h
│   ├── Enemy.h
│   └── Bullet.h
├── Core/
│   ├── GameObject.h
│   ├── Component.h
│   ├── System.h
│   └── Utils.h
├── Main/
│   └── main.cpp
└── Resources/
    ├── Textures/
    │   ├── player.png
    │   ├── enemy.png
    │   └── bullet.png
    ├── Fonts/
    │   └── font.ttf
    └── Sounds/
        ├── shoot.wav
        └── explosion.wav
```
