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
	void updateObjects(float delta); //actualiza la escena y borra los objetos que devuelven false
	void addButterfly(); //añade mariposa con sus parametros
	bool loadLevel(int levelNumber);//lle la configuracion y devuelve false si falta archivo
	void startLevel();//crea arco y mariposas iniciales del nivel
	void clearObjects();//borra los objectos de la escena y pone mainbow a nullptr
	void saveGame() const;// guarda la partida
	void loadGame();//carga la partida
	void loadObjects(std::istream&file);//lee los objetos del guardado y si falla pone la linea de error
	bool readLevelOption(const std::string& option, std::istream& values); // aplica la opcion de nivel y devuelve false si 
	bool readBackgroundColor(std::istream& values); //lee y valida el color del fondo
	void loadObject(const std::string& type, std::istream& values); //construye el objeto a partir del tipo leido

	bool exit;

	
	Bow* mainBow; //está en objects, es para acceder a los eventos y municion
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
	 int checkCollision(const Rectangle& rect); //devuelve el cambio de puntuacion del primer impacto
	 void arrowCreated(); //informa de flecha creada
	 void arrowDestroyed();//informa de flecha destruida
	 void addObject(SceneObject* object); //añade a la escena un objeto que luego Game libera
	 int randomInt(int min, int max); //generador de aleatorios
	 void changeScore(int points); //cambio de puntuacion en funcion de lo que haga el jugador
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
