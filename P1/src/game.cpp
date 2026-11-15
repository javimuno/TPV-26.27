#include "game.h"

#include <string>
#include <SDL3_image/SDL_image.h>
#include <fstream>

#include "texture.h"
#include "Butterfly.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"bow.png", 1, 2},
	{"arrow.png"},
	{"balloons.png",7,6},
	{"butterfly.png",4,10},
};

// Lee las flechas iniciales y el intervalo entre globos.
// Ignora el resto de opciones y lanza una excepcion si un valor no es valido.
void Game::loadLevel()
{


	std::ifstream file(std::string (LEVEL_ROOT) + "1.txt");

	if (!file.is_open()) {
		throw std::string("No se puede abrir el archivo de nivel");
	}

	std::string option;
	
	file >> option;

	while (!file.fail())
	{
		
		

		if (option == "arrows") {
			file >> iniArrows_file;

			if (file.fail()||iniArrows_file<0) {
				throw std::string("Valor de arrows no valido");
			}
		}

		else if (option == "baldelay") {
			int baldelayAux;
			file >> baldelayAux;			

			if (file.fail() || baldelayAux<=0) {
				throw std::string("Valor de baldelay no valido");
			}
			balloonInterval_file = baldelayAux / 1000.0f;
		}

		else if (option == "butterflies") {
			file >> initialButterflies;

			if (file.fail() || initialButterflies < 0) {
				throw std::string("Valor de butterflies no valido");
			}
		}

		else if (option == "bgcolor") {
			int red, green, blue;
			file >> red >> green >> blue;

			if (file.fail() || red < 0 || red>255 ||
				green < 0 || green > 255 || blue < 0 || blue > 255) {
				throw std::string("Valor de bgColor no válido");
			}

			backgroundColor = {
				static_cast<Uint8>(red),
				static_cast<Uint8>(green),
				static_cast<Uint8>(blue),
				255
			};
		}

		else if (option == "balspeed") {
			float ballspeed;
			file >> ballspeed;

			if (file.fail() || ballspeed <= 0.0f) {
				throw std::string("Valor ballSpeed no válido");
			}

			balloonBaseSpeed = ballspeed*1000; //porque viene en px/ms -.-
		}

		// Descarta el resto de la linea antes de leer la siguiente opcion.
		file.ignore(1000, '\n');
		file >> option;
	}
}

Game::Game()
	: exit(false),arrowsInFlight(0),initialButterflies(0),backgroundColor{0,0,0,255},balloonBaseSpeed((65.0f))
{
	//carga del nivel
	loadLevel();

	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		for (Texture* texture : textures) {
			delete texture;
		}

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego

	//arco
	mainBow = new Bow(Rectangle({ 100.0f,300.0f }, 100.0f, 100.0f), Vector2D<>{ 0.0f, 0.0f }, getTexture(BOW),iniArrows_file, false,this);
	
	addObject(mainBow);
	for (int i = 0; i < initialButterflies; i++) {
		addButterfly();
	}

}

//texturas, renderizador y ventana y cierre
Game::~Game()
{

	for (SceneObject* object : objects) {
		delete object;
	}

	for (Texture* texture : textures) {
		delete texture;
	}



	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_SetRenderDrawColor(renderer,backgroundColor.r,backgroundColor.g,backgroundColor.b,backgroundColor.a);
	SDL_RenderClear(renderer);

	for (SceneObject* object : objects) {
		object->render();
	}

	SDL_RenderPresent(renderer);

	
}

void
Game::update(float delta)
{
	
	updateObjects(delta);

	balloonTimer += delta;

	if (balloonTimer >= balloonInterval_file) {
		addBalloon();
		balloonTimer = 0.0f;
	}
	
	mainBow->update(delta);

	//Fin de partida
	if (!mainBow->hasArrows() && arrowsInFlight==0) {
		std::cout << "Fin de partida" << std::endl;
		exit = true;
	}
	
}
 // pulsaciones, actualizaciones, dibujo y espera ms antes de nueva vuelta
void
Game::run()
{
	Uint64 startTime, frameTime;

	while (!exit) {

		startTime = SDL_GetTicks();
		handleEvents();
		update(FRAME_PERIOD/1000.0f);
		render();

		frameTime = SDL_GetTicks() - startTime;

		if(frameTime<FRAME_PERIOD)
		SDL_Delay(FRAME_PERIOD-frameTime);
	}
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		
		mainBow->handleEvent(event);
	}
}

void 
Game::addBalloon()
{

	//colores

	int minC = BALLOON_MIN_COLOR;
	int maxC = BALLOON_MAX_COLOR;

	std::uniform_int_distribution<int> cDistribution(minC, maxC);
	int c = cDistribution(randomGenerator);

	//franja del rango de espacio aleatorio
	float minX = WINDOW_WIDTH * BALLOON_AREA_START;
	float maxX = WINDOW_WIDTH - BALLOON_WIDTH;

	std::uniform_real_distribution<float> xDistribution(minX, maxX);
	float x = xDistribution(randomGenerator);

	//velocidades aleatorias

	const float minV = balloonBaseSpeed * BALLOON_MIN_SPEED_RATIO;
	const float maxV = balloonBaseSpeed * BALLOON_MAX_SPEED_RATIO;


	//esto con mi sistema de rango aleatorio
	std::uniform_real_distribution<float> vDistribution(minV, maxV);
	float v = vDistribution(randomGenerator);	

	//creacion
	addObject(new Balloon(this,Rectangle({x,WINDOW_HEIGHT }, BALLOON_WIDTH, BALLOON_WIDTH), Vector2D<>(0, -v), getTexture(BALLOON), c));
}

// actualiza los objectos de la escena y elimina los que han terminado su recorrido o explosion.
void Game::updateObjects(float delta)
{

	auto it = objects.begin();

	while (it!=objects.end()) 
	{
		if (!(*it)->update(delta)) {
			delete *it;
			it=objects.erase(it);				
		}
		else
		{
			++it;
		}
	}
}

// Busca el primer globo alcanzado y suma puntos una sola vez por globo.
 int  Game::checkCollision(const Rectangle& rect)
 {
	 const int scoreBeforeHit = score;

	 for (SceneObject* object : objects) {
		 if (object->hit(rect)) {			
			 return score-scoreBeforeHit;
		}
	}
	 return 0;
 }

 void Game::arrowCreated()
 {
	 ++arrowsInFlight;
 }

 void Game::arrowDestroyed()
 {
	 --arrowsInFlight;
 }

 void Game::addObject(SceneObject* object)
 {
	 objects.push_back(object);
 }


 void Game::addButterfly() 
 {

	 const int BUTTERFLY_WIDTH = 60;
	 const int BUTTERFLY_HEIGHT = 46;

	 const float x = static_cast<float>(randomInt(0, WINDOW_WIDTH - BUTTERFLY_WIDTH));
	 const float y = static_cast<float>(randomInt(0, WINDOW_WIDTH - BUTTERFLY_HEIGHT));
	 float velX = static_cast<float>(randomInt(25, 80));
	 float velY = static_cast<float>(randomInt(20, 50));

	 if (randomInt(0, 1) == 1) {
		 velX = -velX;
	 }
	 if (randomInt(0, 1) == 1) {
		 velY = -velY;
	 }

	 const Rectangle rect({ x,y}, 60.0f, 46.0f);
	 const Vector2D<> velocity(velX,velY);

	 addObject(new Butterfly(this, rect, velocity, getTexture(BUTTERFLY)));

 }

 int Game::randomInt(int min, int max)
 {
	 std::uniform_int_distribution<int> distribution(min, max);
	 return distribution(randomGenerator);
 }

 void Game::changeScore(int points)
 {
	 score += points;
	 cout << "Puntos totales: " << score << endl;
 }

 
