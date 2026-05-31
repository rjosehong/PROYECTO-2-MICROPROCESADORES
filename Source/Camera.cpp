#include "Camera.h"

/// Constructor.
Camera::Camera(float zoomLevel)
    : zoomLevel(zoomLevel)
{
}

/// Retorna el tamaño visible actual.
sf::Vector2f Camera::GetViewSize()
{
    return viewSize;
}

/// Genera la vista principal del juego.
sf::View Camera::GetView(sf::Vector2u windowSize)
{
    // Relación ancho/alto de la ventana.
    float aspect = (float)windowSize.x / (float)windowSize.y;

    // Ajuste automático del zoom.
    if (aspect < 1.0f)
    {
        viewSize = sf::Vector2f(
            zoomLevel,
            zoomLevel / aspect
        );
    }
    else
    {
        viewSize = sf::Vector2f(
            zoomLevel * aspect,
            zoomLevel
        );
    }

    // Devuelve la vista centrada en la posición actual.
    return sf::View(position, viewSize);
}

/// Genera la vista usada para elementos UI.
sf::View Camera::GetUIView()
{
    float aspect = viewSize.x / viewSize.y;

    viewSize = sf::Vector2f(
        100.0f,
        100.0f / aspect
    );

    return sf::View(sf::Vector2f(), viewSize);
}