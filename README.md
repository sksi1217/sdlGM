# sdl2


Алгоритм Брезенхэма использую для рисования круга:
https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm


Elliptic orbit:
https://en.m.wikipedia.org/wiki/Elliptic_orbit

Гомановская траектория:
https://en.wikipedia.org/wiki/Hohmann_transfer_orbit

PACING CURVE - нужно для того чтобы была интенсивность собития сложности (пейсинг)

---

```
AttributesComponent.h   - Основные характеристики (сила, ловкость, интеллект)
CombatComponent.h       - Боевые характеристики (точность, уклонение, сопротивление)
EnergyComponent.h       - Энергия/мана и её регенерация
AbilityComponent.h      - Пассивные предметы
EffectComponent.h       - Временные эффекты (BUFFS/DEBUFFS)
ResourceGatheringComponent.h - Сбор ресурсов
LevelComponent.h        - Уровень и опыт персонажа
```

# Игрок (пример)
#### Основные характеристики:
- [x] Здоровье (HP)  
- [ ] Энергия/Мана (Energy/Mana)
- [ ] Броня (Armor)
- [ ] Сила (Strength)
	- **Влияние** :
	    - Здоровье на 5.
	    - Восстановление здоровья на 0,1.
	    - Влияет на шанс критического удара.
    	    - Вампиризм от атак, зависит от здоровья врага.
- [ ] Ловкость (Agility)
	- **Влияние** :
	    - Увеличивает шанс уклонения от атак противника.
	    - Повышает точность стрельбы или дальность атак.
	    - Может увеличивать скорость передвижения.
	    - Броню на (1/6).
	    - Скорость атаки на 1.
- [ ] Интеллект (Intelligence)
	- **Влияние** :
   		- снижать время перезарядки способностей
		- Увеличивает сбор на 10.
  		- Увеличивает радиус подбора на 5.
```
 	- (- Увеличивает урон от магических атак.
	    - Повышает максимальный запас маны/энергии и скорость её восстановления.
```
	    
- [x] Скорость передвижения (Movement Speed)  
- [x] Скорость атаки (Attack Speed)  

#### Боевые характеристики:
- [x] Урон (Damage)  
- [x] Критический удар (Critical Hit Chance)  
- [x] Множитель критического удара (Critical Damage Multiplier)  
- [ ] Точность (Accuracy)  
- [ ] Уклонение (Dodge Chance)  
- [ ] Проникновение брони (Armor Penetration)  - возможно, но не уверен
- [ ] Сопротивление урону (Damage Resistance)
	- Снижает получаемый урон от различных типов атак (физический, магический, огненный и т.д.).
- [x] Регенерация здоровья (Health Regeneration)  
- [ ] Регенерация энергии/маны (Energy/Mana Regeneration)  
#### Способности:
- [ ] Активные способности (Active Abilities)  
- [ ] Пассивные способности (Passive Abilities)  
- [ ] Кулдаун способностей (Ability Cooldowns) - всех оружий 
- [ ] Эффекты способностей (Ability Effects)  

#### Взаимодействие с окружающей средой:
- [ ] Сбор ресурсов (Resource Gathering) 
	- радиус подбора опыта

#### Временные эффекты:
- [ ] Слабости (Debuffs)
	-20% к скорости передвижении (пример)
- [ ] Усиления (Buffs)
- [ ] Время действия эффектов (Effect Duration)  

#### Прочее:
- [ ] Уровень персонажа (Level)  
- [ ] Опыт (Experience Points)  
- [ ] Размер персонажа (Character Size)  


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
