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
	static constexpr int BALLOON_WIDTH = 64.0f;

	// Timer para el spawn de balloons y acumulador
	static constexpr float BALLOON_INTERVAL = 0.5f;
	float balloonTimer = 0.0f;
	void drawDebugRect(const Rectangle& rect) const;

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

	

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego

	std::vector<Balloon*> balloons;
	Bow* mainBow;
	std::vector<Arrow*> arrows;

public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	//cargar flecha
	void addArrow(const Rectangle& rect, const Vector2D<>& vel);

	// Comprueba si hay algún objeto colocado en ese rectángulo
	 bool checkCollision(const Rectangle& rect);
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
