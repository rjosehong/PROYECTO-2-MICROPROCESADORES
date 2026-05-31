#pragma once

#include <SFML/Graphics.hpp>

/// Controla la cámara del juego.
class Camera
{
public:

    /// Constructor.
    /// @param zoomLevel tamaño visible de la cámara.
    Camera(float zoomLevel = 5.0f);

    /// Obtiene el tamaño actual de la vista.
    sf::Vector2f GetViewSize();

    /// Construye la vista principal del juego.
    sf::View GetView(sf::Vector2u windowSize);

    /// Construye la vista usada para UI.
    sf::View GetUIView();

    /// Nivel de zoom.
    float zoomLevel;

    /// Posición de la cámara.
    sf::Vector2f position;

private:

    /// Tamaño actual de la vista.
    sf::Vector2f viewSize{};
};