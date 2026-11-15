#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>
#include <list>

#include "Balloon.h"
#include "Bow.h"
#include "Arrow.h"

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
	

	// Timer para el spawn de balloons y acumulador
	//static constexpr float BALLOON_INTERVAL = 0.5f;
	

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
		BUTTERFLY,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Color backgroundColor;
	std::array<Texture*, NUM_TEXTURES> textures;
	std::mt19937_64 randomGenerator{ std::random_device{}() };

	static constexpr float BALLOON_WIDTH = 64.0f;
	static constexpr int BALLOON_MIN_COLOR = 0;
	static constexpr int BALLOON_MAX_COLOR = 6;
	static constexpr float BALLOON_MIN_SPEED_RATIO = 0.8f; //80%
	static constexpr float BALLOON_MAX_SPEED_RATIO = 1.2f; //120%
	static constexpr float BALLOON_AREA_START = 1.0f / 3.0f;

	float balloonBaseSpeed=0.0f;
	int currentLevel = 1;
	int levelGoal=0;

	

	void render() const;
	void update(float delta);
	void handleEvents();
	void addBalloon();
	void updateObjects(float delta);
	void addButterfly();
	bool loadLevel(int levelNumber);
	void startLevel();
	void clearObjects();
	void saveGame() const;
	void loadGame();
	void loadObjects(std::istream&file);

	bool exit;

	

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego

	//está en objects, es para acceder a los eventos y municion
	Bow* mainBow;
	std::list<SceneObject*> objects;
	

	float balloonTimer = 0.0f;
	int score=0;
	int iniArrows_file=10;
	float balloonInterval_file=0.5f;
	int arrowsInFlight;
	int initialButterflies;


	

public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	 int checkCollision(const Rectangle& rect);
	 void arrowCreated();
	 void arrowDestroyed();
	 void addObject(SceneObject* object);
	 int randomInt(int min, int max);
	 void changeScore(int points);
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
