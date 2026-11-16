#include "Butterfly.h"
#include "texture.h"
#include "game.h"

Butterfly::Butterfly(Game* game, const Rectangle& rect, const Vector2D<>& velocity, Texture* texture) : 
MobileObject(game, rect, velocity, texture),
falling(false),
directionTimer(0.0f),
animationRow(velocity.getX() < 0.0f ? LEFT_FLAP_ROW : RIGHT_FLAP_ROW),
flapTimer(0.0f),
animBack(1)
{}

void Butterfly::render() const
{
	getTexture()->renderFrame(getRect().to_sdl(), animationRow, col);

}

bool Butterfly::update(float delta)
{	
	//frame sin cambio y eliminacion al llegar abajo de la pantalla
	if (falling) {
		MobileObject::update(delta);
		return getRect().getTopLeftCorner().getY() < Game::WINDOW_HEIGHT;
	}

	updateAnimation(delta);
	directionTimer += delta;

	if (directionTimer >= DIRECTION_CHANGE_INTERVAL) {
		directionTimer = 0.0f;
		int direcionChoice = getGame()->randomInt(0, 2);

		if (direcionChoice == 0) {
			reverseHorizontalDirection();
		}
		else if (direcionChoice == 1) {
			reverseVerticalDirection();
		}
		else
		{
			reverseHorizontalDirection();
			reverseVerticalDirection();
		}
	}

	MobileObject::update(delta);	
	//rebote apartado de aqui para acortar el metodo
	bounceAtEdges();

	if (movingLeft()) {
		animationRow = LEFT_FLAP_ROW;
	}
	else
	{
		animationRow = RIGHT_FLAP_ROW;
	}	
	return true;
}


void Butterfly::bounceAtEdges() //rebote en los bordes
{

	const float x = getRect().getTopLeftCorner().getX();
	const float maxX = Game::WINDOW_WIDTH - getRect().getWidth();
	const float y = getRect().getTopLeftCorner().getY();
	const float maxY = Game::WINDOW_HEIGHT - getRect().getHeight();

	if (x < 0.0f) {
		move(Vector2D<>(-x, 0.0f));
		reverseHorizontalDirection();
	}
	else if (x > maxX) {
		move(Vector2D<>(maxX - x, 0.0f));
		reverseHorizontalDirection();
	}

	if (y < 0.0f) {
		move(Vector2D<>(0.0f, -y));
		reverseVerticalDirection();
	}
	else if (y > maxY) {
		move(Vector2D<>(0.0f, maxY - y));
		reverseVerticalDirection();
	}
}

bool Butterfly::hit(const Rectangle& tip)
{
	if (getRect().hasIntersection(tip)&&falling==false) {
		falling = true;
		setVelocity(Vector2D<>(0.0f, FALL_SPEED));
		animationRow = 3;
		col = 5;
		getGame()->changeScore(HIT_PENALTY);
		return true;
	}
	else
	{
		return false;
	}
	
}

void Butterfly::save(std::ostream& out) const
{
	out << "butterfly ";
	MobileObject::save(out);
	out << ' ' << falling << ' '
		<< directionTimer << ' '
		<< animationRow << ' '
		<< col << ' '
		<< flapTimer << ' '
		<< animBack << '\n';

}

Butterfly::Butterfly(std::istream& in, Game* game)
	: MobileObject(game, in, game->getTexture(Game::BUTTERFLY)),
	falling(false), directionTimer(0.0f), animationRow(RIGHT_FLAP_ROW),
	flapTimer(0.0f), animBack(1)
{
	if (!(in >> falling >> directionTimer >> animationRow
		>> col >> flapTimer >> animBack)) {
		throw GameError("Datos de la mariposa no validos");
	}
}

void Butterfly::updateAnimation(float delta)
{
	flapTimer += delta;
	

	if (flapTimer >= FLAP_FRAME_TIME) {
		flapTimer = 0.0f;
		col += animBack;

		if (col == FLAP_FRAMES - 1) {
			animBack = -1;
		}
		else if (col==0)
		{
			animBack = 1;
		}
	}
	
}


