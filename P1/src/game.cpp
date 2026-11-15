#include "game.h"

#include <string>
#include <SDL3_image/SDL_image.h>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "texture.h"
#include "Butterfly.h"
#include "GameError.h"

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
bool Game::loadLevel(int levelNumber)
{

	const std::string filename = std::string((LEVEL_ROOT)+std::to_string(levelNumber)+".txt");

	std::ifstream file(filename);
	//std::ifstream file(std::string(LEVEL_ROOT)+"1.txt");

	if (!file.is_open()) {

		if (!std::filesystem::exists(filename)) {
			return false;
		}
		throw std::string("No se puede abrir: " + filename);
	}

	//reincio
	levelGoal = 0; 
	exit = false;	
	iniArrows_file = 0;
	initialButterflies = 0;
	backgroundColor = { 0,0,0,255 };
	balloonBaseSpeed = 65.0f;

	std::string option;
	std::string line;
	int lineNumber = 0;
	
	file >> option;


	while (std::getline(file,line))
	{
		++lineNumber;
		std::istringstream values{ line };

		std::string option;
		if (!(values >> option) || option[0] == '#') {
			continue;
		}
		
		

		if (option == "arrows") {
			values >> iniArrows_file;

			if (values.fail()||iniArrows_file<0) {
				throw FileFormatError(filename, lineNumber, "Valor de arrows no valido");
			}
		}

		else if (option == "baldelay") {
			int baldelayAux;
			values >> baldelayAux;			

			if (values.fail() || baldelayAux<=0) {
				throw FileFormatError(filename, lineNumber, "Valor de baldelay no valido");
			}
			balloonInterval_file = baldelayAux / 1000.0f;
		}

		else if (option == "butterflies") {
			values >> initialButterflies;

			if (values.fail() || initialButterflies < 0) {
				throw FileFormatError(filename, lineNumber, "Valor de butterflies no valido");
			}
		}

		else if (option == "bgcolor") {
			int red, green, blue;
			values >> red >> green >> blue;

			if (values.fail() || red < 0 || red>255 ||
				green < 0 || green > 255 || blue < 0 || blue > 255) {
				throw FileFormatError(filename, lineNumber, "Valor de bgColor no valido");
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
			values >> ballspeed;

			if (values.fail() || ballspeed <= 0.0f) {
				throw FileFormatError(filename, lineNumber, "Valor ballSpeed no valido");
			}

			balloonBaseSpeed = ballspeed*1000; //porque viene en px/ms -.-
		}

		else if (option == "goal") {
			values >> levelGoal;

			if (values.fail() || levelGoal < 0) {
				throw FileFormatError(filename, lineNumber, "Valor de goal no válido");
			}
			

		}
		// Descarta el resto de la linea antes de leer la siguiente opcion.
		
	}

	return true;
}

void Game::startLevel()
{
	mainBow = new Bow(Rectangle({ 100.0f,300.0f }, 100.0f, 100.0f), Vector2D<>{ 0.0f, 0.0f }, getTexture(BOW), iniArrows_file, false, this);

	addObject(mainBow);
	for (int i = 0; i < initialButterflies; i++) {
		addButterfly();
	}
}

void Game::clearObjects()
{
	for (SceneObject* object : objects) {
		delete object;
	}	

	objects.clear();
	mainBow = nullptr;
}

void Game::saveGame() const
{
	std::ofstream file{ "saved.txt" };//crea o reemplaza contenido si existe

	if (!file) {
		throw GameError("No se puede escribir en saved.txt");
	}

	file << currentLevel << ' ' << score << ' ' << balloonTimer << '\n';
	
	//todos los objetos guardan su estado
	for (const SceneObject* object : objects) {
		object->save(file);
	}
	file.close();

	if (!file) {
		throw GameError("No se ha podido hacer el guardado");
	}

}

Game::Game()
	: exit(false),arrowsInFlight(0),initialButterflies(0),backgroundColor{0,0,0,255},balloonBaseSpeed((65.0f))
{
	//carga del nivel
	//loadLevel();
	if (!loadLevel(currentLevel)) { throw std::string("Falta el nivel 1"); }

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

	
	startLevel();

}

//texturas, renderizador y ventana y cierre
Game::~Game()
{
	clearObjects();

	//porque sino no hay texturas en el cambio de niveles
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
	
	if (levelGoal > 0 && score >= levelGoal) {
		if (loadLevel(currentLevel + 1)) {
			++currentLevel;
			clearObjects();
			startLevel();
		}
		else
		{
			std::cout << "Victoria\n";
			exit = true;
		}
		return;
	}
	

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

		if (event.type == SDL_EVENT_KEY_DOWN) {
			if ((event.key.mod & SDL_KMOD_CTRL) != 0 && event.key.key == SDLK_S ) {
				if (!event.key.repeat) { //1 vez solo
					saveGame();
				}

				continue;
			}
		}
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

	 const float x = static_cast<float>(randomInt(WINDOW_WIDTH*1/3, WINDOW_WIDTH - BUTTERFLY_WIDTH));
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

 
