// Game.cpp
#include "Game.h"
#include <iostream>
#include "project/Components/TransformComponent.h"
#include "project/Core/GameObject.h"
#include "project/Entities/Player.h"
#include "project/Utils/TextureLoader.h"
#include "project/Systems/CollisionSystem.h"
#include "project/Entities/Skelet.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameObject> enemy;
std::shared_ptr<GameObject> enemy1;
std::shared_ptr<GameObject> enemy2;
std::shared_ptr<GameObject> enemy3;

Camera camera = { {SCREEN_WIDTH, SCREEN_HEIGHT} };

std::shared_ptr<GameObject> weapon;

CollisionSystem сollisionSystem;

// Designer
Game::Game(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		SDL_Quit();
		isRunning = false;
		return;
	}


	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}

	ManagerGame::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!ManagerGame::renderer) {
		std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}
}

// Destructor
Game::~Game() {
	std::cout << "Destroying renderer and window..." << std::endl;
	SDL_DestroyRenderer(ManagerGame::renderer);
	SDL_DestroyWindow(window);

	std::cout << "Quitting SDL_image and SDL..." << std::endl;
	IMG_Quit();
	SDL_Quit();
}


// Initialization
void Game::Initialize() {
	// Установка базового пути к ресурсам
	TextureLoader& textureLoader = TextureLoader::GetInstance();
	textureLoader.SetBasePath("project/Resources/Textures");

	// Создание текстуры через TextureLoader
	SDL_Texture* boxTexture = TextureLoader::GetInstance().LoadTexture("Atlas.png", ManagerGame::renderer);

	std::cout << "Initialized Weapons!" << std::endl;
	weapon = std::make_shared<Weapon>(boxTexture);

	std::cout << "Game initialized!" << std::endl;
}

// Resource Loading
void Game::LoadContent() {
	std::cout << "Loading content..." << std::endl;

	// Создание текстуры через TextureLoader
	SDL_Texture* playerTexture = TextureLoader::GetInstance().LoadTexture("player.png", ManagerGame::renderer);
	SDL_Texture* boxTexture = TextureLoader::GetInstance().LoadTexture("box.png", ManagerGame::renderer);
	SDL_Texture* enemyTexture = TextureLoader::GetInstance().LoadTexture("skelet.png", ManagerGame::renderer);

	// Создание игрока
	player = std::make_shared<Player>(SDL_FPoint{ 0, 0 }, playerTexture);
	ManagerGame::objects.push_back(player);

	
	auto playerTransform = player->GetComponent<TransformComponent>();
	
	enemy = std::make_shared<Skelet>(SDL_FPoint{ 50, 0 }, enemyTexture, playerTransform);
	enemy1 = std::make_shared<Skelet>(SDL_FPoint{ 16, 16 }, enemyTexture, playerTransform);
	enemy2 = std::make_shared<Skelet>(SDL_FPoint{ 0, 64 }, enemyTexture, playerTransform);
	enemy3 = std::make_shared<Skelet>(SDL_FPoint{ 0, 16 }, enemyTexture, playerTransform);

	/*
	ManagerGame::objects.push_back(enemy);
	ManagerGame::objects.push_back(enemy1);
	ManagerGame::objects.push_back(enemy2);
	ManagerGame::objects.push_back(enemy3);
	

	ManagerGame::enemies.push_back(enemy);
	ManagerGame::enemies.push_back(enemy1);
	ManagerGame::enemies.push_back(enemy2);
	ManagerGame::enemies.push_back(enemy3);
	*/


	ManagerGame::_allWeapons.push_back(weapon);
}

// Logic Update
void Game::Update(float deltaTime) 
{
	pacingSystem.Update(deltaTime);

	if (pacingSystem.shouldSpawnWave()) {
		spawnWave(pacingSystem.enemiesPerWave);
		pacingSystem.resetTimer();
	}

	for (size_t i = 0; i < ManagerGame::objects.size(); ) {
		auto& obj = ManagerGame::objects[i];

		auto statis = obj->GetComponent<StateComponent>();

		// Проверяем, активен ли объект
		if (!statis->IsActive) {
			// Удаляем объект, если он неактивен
			ManagerGame::objects.erase(ManagerGame::objects.begin() + i);
		}
		else {
			// Обновляем объект, если он активен
			obj->Update(deltaTime);
			++i; // Переходим к следующему объекту
		}
	}

	сollisionSystem.Update();

	camera.UpdateCamera(player->GetComponent<TransformComponent>()->Position, deltaTime);
}

// Frame rendering
void Game::Draw() {
	SDL_SetRenderDrawColor(ManagerGame::renderer, 50, 50, 50, 255);
	SDL_RenderClear(ManagerGame::renderer);

	// Drawing of all objects
	for (auto& obj : ManagerGame::objects) {
		obj->Draw(ManagerGame::renderer, camera);
	}

	SDL_RenderPresent(ManagerGame::renderer);
}

// Выгрузка ресурсов
void Game::UnloadContent() {
	std::cout << "Unloading content..." << std::endl;
}

// Game.cpp
void Game::HandleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
			return; // Выходим из метода сразу после получения события SDL_QUIT
		}
	}
}

void Game::Run() {
	Initialize();
	LoadContent();

	const int TARGET_FPS = 60;
	const Uint32 TARGET_FRAME_TIME = 1000 / TARGET_FPS; // Time of one frame in milliseconds

	Uint32 lastTime = SDL_GetTicks(); // Time of previous frame

	while (isRunning) {
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f; // Time difference between frames
		lastTime = currentTime;

		// Logic Update
		Update(deltaTime);

		Draw();

		HandleEvents();

		// Restriction FPS
		Uint32 frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < TARGET_FRAME_TIME) {
			SDL_Delay(TARGET_FRAME_TIME - frameTime);
		}
	}

	UnloadContent();
}


SDL_Rect Game::generateSpawnPoint() const {
	SDL_Rect spawnPoint { };
	int side = rand() % 4; // Случайная сторона
	switch (side) {
	case 0: // Слева
		spawnPoint.x = -32; // Размер врага
		spawnPoint.y = rand() % SCREEN_HEIGHT;
		break;
	case 1: // Справа
		spawnPoint.x = SCREEN_WIDTH + 32;
		spawnPoint.y = rand() % SCREEN_HEIGHT;
		break;
	case 2: // Сверху
		spawnPoint.x = rand() % SCREEN_WIDTH;
		spawnPoint.y = -64;
		break;
	case 3: // Снизу
		spawnPoint.x = rand() % SCREEN_WIDTH;
		spawnPoint.y = SCREEN_HEIGHT + 64;
		break;
	}
	return spawnPoint;
}

void Game::spawnWave(int count) {
	SDL_Texture* enemyTexture = TextureLoader::GetInstance().LoadTexture("skelet.png", ManagerGame::renderer);
	auto playerTransform = player->GetComponent<TransformComponent>();

	for (int i = 0; i < count; ++i) {
		SDL_Rect pos = generateSpawnPoint();
		auto enemy = std::make_shared<Skelet>(
			SDL_FPoint{ static_cast<float>(pos.x), static_cast<float>(pos.y) },
			enemyTexture,
			playerTransform
		);
		ManagerGame::objects.push_back(enemy);
		ManagerGame::enemies.push_back(enemy);
	}
}