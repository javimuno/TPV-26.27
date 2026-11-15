#include "InfoBar.h"
#include "game.h"
#include <string>

void InfoBar::renderArrows() const
{
    // Obtener la textura de las flechas
    Texture* arrowTexture = game->getTexture(Game::ARROW);

    // Dimensiones de cada flecha
    float arrowWidth = arrowTexture->getFrameWidth() * 0.10f;
    float arrowHeight = arrowTexture->getFrameHeight() * 0.10f;

    float width = arrowHeight;
    float height = arrowWidth;

    // Posición inicial en la esquina superior izquierda
    float xA = 5.0f;
    float yA = 20.0f;

    // Separación entre flechas
    float spacing = 10.0f;

    // Dibujar tantas flechas como queden
    for (int i = 0; i < game->getArrows(); i++) {
        SDL_FRect dest = {
            xA,
            yA,
            arrowWidth,
            arrowHeight
        };

        arrowTexture->render(dest, -90.0);

        xA += arrowHeight + spacing;
    }
}

void InfoBar::renderScore() const
{
    // Obtener la textura de los números
    Texture* digits = game->getTexture(Game::DIGITS);

    // Convertir la puntuación a texto
    std::string scoreText = std::to_string(game->getScore());

    // Calcular las dimensiones de cada cifra
    float digitWidth = static_cast<float>(digits->getFrameWidth());
    float digitHeight = static_cast<float>(digits->getFrameHeight());

    // Calcular la posición inicial para alinearlo a la derecha
    float margin = 10.0f;
    float xD = Game::WINDOW_WIDTH - margin -
        scoreText.length() * digitWidth;

    float yD = margin;

    // Dibujar cada cifra
    for (char digit : scoreText) {
        int frame = digit - '0';

        SDL_FRect dest = {
            xD,
            yD,
            digitWidth,
            digitHeight
        };

        digits->renderFrame(dest, 0, frame);

        xD += digitWidth;
    }
}

InfoBar::InfoBar(Game* game)
    : GameObject(game) {}

bool InfoBar::update(float dt)
{
    return true;
}

void InfoBar::render() const
{
    renderArrows();
    renderScore();
}
