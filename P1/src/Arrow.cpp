#include "Arrow.h"
#include "game.h"

Arrow::Arrow(Texture* text, const Point2D<float>& pos, const Vector2D<float>& vel, Game* g) 
	: texture(text), position(pos), velocity(vel), game(g)
{
	int w = float(texture->getFrameWidth());
	int h = float(texture->getFrameHeight());
	rect = new Rectangle(pos, w * Game::GAME_SCALE, h * Game::GAME_SCALE);
}

Arrow::~Arrow()
{
	delete rect;
}

void Arrow::render() const
{
	texture->render(rect->to_sdl());

	// Dibujar la caja de colisión en verde.
	/*SDL_SetRenderDrawColor(game->getRenderer(), 0, 255, 0, 255);
	SDL_FRect box = tip.to_sdl();
	SDL_RenderRect(game->getRenderer(), &box);*/
}

bool Arrow::update(float delta)
{
	rect->move(velocity * delta);

	float posLeft = rect->getTopLeftCorner().getX();

	// Rectángulo pequeño en la punta derecha de la flecha.
	float tipWidth = rect->getWidth() * 0.20f;
	
	Point2D<float> tipPosition(
		rect->getTopLeftCorner().getX() + rect->getWidth() - tipWidth,
		rect->getTopLeftCorner().getY()
	);

	Rectangle tip(tipPosition, tipWidth, rect->getHeight());
	//tip = Rectangle(tipPosition, tipWidth, rect->getHeight());
	

	game->checkCollision(tip);
	return posLeft < Game::WINDOW_WIDTH;
}
