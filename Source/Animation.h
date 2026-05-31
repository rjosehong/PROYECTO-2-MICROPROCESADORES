#pragma once

// Librería SFML para manejo de texturas
#include <SFML/Graphics.hpp>

/// Representa un frame individual de una animación.
struct AnimFrame
{
    /// Constructor del frame.
    /// @param time instante en el que este frame debe mostrarse.
    /// @param texture textura correspondiente al frame.
    AnimFrame(float time = 0.0f, sf::Texture texture = sf::Texture())
        : time(time), texture(texture)
    {
    }

    /// Tiempo asociado al frame.
    float time = 0.0f;

    /// Imagen que se mostrará en este frame.
    sf::Texture texture;
};

/// Clase encargada de gestionar animaciones.
class Animation
{
public:

    /// Constructor de la animación.
    /// @param lenght duración total de la animación.
    /// @param frames conjunto de frames.
    Animation(float lenght = 0.0f, std::vector<AnimFrame> frames = {});

    /// Actualiza el tiempo interno de la animación.
    /// @param deltaTime tiempo transcurrido desde el último frame.
    void Update(float deltaTime);

    /// Obtiene la textura correspondiente al frame actual.
    sf::Texture GetTexture();

private:

    /// Tiempo acumulado de reproducción.
    float time = 0.0f;

    /// Duración total de la animación.
    float lenght;

    /// Lista de frames.
    std::vector<AnimFrame> frames;
};