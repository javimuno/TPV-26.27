#include "Butterfly.h"
#include "game.h"

Butterfly::Butterfly(Texture* text, const Point2D<float>& pos, const Vector2D<float>& dim, const Vector2D<float>& vel, Game* g)
	: MobileObject(text, pos, dim, vel, g), 
    dead(false), 
    directionTimer(0.0f), 
    nextDirectionChange(randomDirectionDelay()),
    animationFrame(0),
    animationTimer(0.0f) {}

Butterfly::Butterfly(std::istream& is, Game* g)
    : MobileObject(is, g->getTexture(Game::BUTTERFLY), g),
    dead(false),
    directionTimer(0.0f),
    nextDirectionChange(0.0f),
    animationFrame(0),
    animationTimer(0.0f)
{
    if (!(is >> dead
        >> directionTimer
        >> nextDirectionChange
        >> animationFrame
        >> animationTimer))
        throw std::string("Error al leer los datos de Butterfly");
}

bool Butterfly::hit(const Rectangle& other)
{
    if (!dead && rect->hasIntersection(other))
    {
        dead = true;
        game->addScore(-5);
        return true;
    }

    return false;
}

bool Butterfly::specificUpdate(float delta)
{
    if (dead)
    {
        animationFrame = 0;
        animationTimer = 0.0f;

        // Caer verticalmente
        Vector2D<float> fallVelocity(0.0f, FALL_SPEED);
        rect->move(fallVelocity * delta);

        float bottomY = rect->getTopLeftCorner().getY() + rect->getHeight();

        // Destruir cuando salga completamente de la ventana
        return bottomY < Game::WINDOW_HEIGHT;
    }

    // Comprobar los límites de la ventana
    float left = rect->getTopLeftCorner().getX();
    float top = rect->getTopLeftCorner().getY();
    float right = left + rect->getWidth();
    float bottom = top + rect->getHeight();

    if (left <= 0 && speed.getX() < 0)
        speed.setX(-speed.getX());

    if (right >= Game::WINDOW_WIDTH && speed.getX() > 0)
        speed.setX(-speed.getX());

    if (top <= 0 && speed.getY() < 0)
        speed.setY(-speed.getY());

    if (bottom >= Game::WINDOW_HEIGHT && speed.getY() > 0)
        speed.setY(-speed.getY());

    // Cambiar aleatoriamente de dirección cada cierto tiempo
    directionTimer += delta;

    if (directionTimer >= nextDirectionChange)
    {
        directionTimer = 0.0f;

        // Generar una nueva dirección aleatoria
        float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159265f;

        float velocity = sqrt(
            speed.getX() * speed.getX() +
            speed.getY() * speed.getY()
        );

        speed.setX(cos(angle) * velocity);
        speed.setY(sin(angle) * velocity);

        nextDirectionChange = randomDirectionDelay();
    }

    animationTimer += delta;

    if (animationTimer >= ANIMATION_FRAME_TIME)
    {
        animationTimer -= ANIMATION_FRAME_TIME;

        animationFrame++;

        if (animationFrame >= NUM_ANIMATION_FRAMES)
            animationFrame = 0;
    }

    return true;
}

float Butterfly::randomDirectionDelay()
{
    return MIN_DIRECTION_DELAY +
        static_cast<float>(rand()) / RAND_MAX *
        (MAX_DIRECTION_DELAY - MIN_DIRECTION_DELAY);
}

bool Butterfly::shouldMove() const
{
    return !dead;
}

void Butterfly::render() const
{
    if (dead)
    {
        texture->renderFrame(rect->to_sdl(), 2, 0);
    }
    else if (speed.getX() < 0)
    {
        texture->renderFrame(rect->to_sdl(), 0, animationFrame);
    }
    else
    {
        texture->renderFrame(rect->to_sdl(), 1, animationFrame);
    }
}

void Butterfly::save(std::ostream& os) const {
    os << "BUTTERFLY ";
    MobileObject::save(os);

    os << dead << " "
        << directionTimer << " "
        << nextDirectionChange << " "
        << animationFrame << " "
        << animationTimer << "\n";
}
