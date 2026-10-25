#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>

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
	static constexpr float GAME_SCALE = 0.25f;
	static constexpr float MAX_SPEED_BALLONS = 200.0f;
	static constexpr float MIN_SPEED_BALLONS = 150.0f;

	static constexpr int NUM_FRAMES_BALLONS = 6;

	float balloonDelay = 0.0f;
	float timerBallons = 0.0f;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
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

	bool exit;
	int numArrows = 0;

	Uint8 bgR = 0;
	Uint8 bgG = 0;
	Uint8 bgB = 0;

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego
	std::vector<Balloon*> balloons;
	std::vector<Arrow*> arrows;
	Bow* playerBow;

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
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
