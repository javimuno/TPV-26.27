#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>
#include <list>

#include "Bow.h"
#include "InfoBar.h"
#include "SceneObject.h"

// #include "rectangle.h"

// Declaraciones anticipadas
class Texture;

/**
 * Clase principal del juego.
 */
class Game
{
public:
	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;
	static constexpr float GAME_SCALE = 0.25f;
	static constexpr float BUTTERFLY_SCALE = 0.5f;
	static constexpr float MAX_SPEED_BALLONS = 200.0f;
	static constexpr float MIN_SPEED_BALLONS = 150.0f;

	static constexpr int NUM_FRAMES_BALLONS = 6;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
		BUTTERFLY,
		DIGITS,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;
	std::mt19937_64 randomGenerator{ std::random_device{}() };

	void render() const;
	void update(float delta);
	void handleEvents();
	void addBalloon();
	void createButterflies();
	Point2D<float> randomPosition() const; 
	Vector2D<float> randomVelocity() const; 

	bool exit;
	int numArrows = 0;

	Uint8 bgR = 0;
	Uint8 bgG = 0;
	Uint8 bgB = 0;

	float balloonDelay = 0.0f;
	float timerBallons = 0.0f;
	float balloonSpeed = 0.0f;
	int balloonPoints = 0; 
	int score = 0;
	int numButterflies = 0.0f;
	int currentLevel;
	int goal;
	bool gameOver;

	// Elemento del juego
	std::list<SceneObject*> sceneObjects;
	Bow* playerBow;

	InfoBar* infoBar = nullptr;

	void saveGame() const;

	void loadGame();
public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	SDL_Renderer* getRenderer() { return renderer; };

	// Ejecuta el bucle principal del juego
	void run();

	//cargar flecha
	void addArrow(const Vector2D<float>& pos, const Vector2D<float>& vel);

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	void loadLevel(const int numLevel);

	void updateNumArrows();

	void addScore(int points);

	int getScore();

	int getArrows() const;
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
