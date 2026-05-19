#include "Camera.h"

Camera::Camera(float zoomLevel)
    : zoomLevel(zoomLevel)
{
}

sf::Vector2f Camera::GetViewSize()
{
    return viewSize;
}

sf::View Camera::GetView(sf::Vector2u windowSize)
{
    float aspect = (float)windowSize.x / (float)windowSize.y;
    sf::Vector2f size;
    if (aspect < 1.0f)
    {
        viewSize = sf::Vector2f(zoomLevel, zoomLevel / aspect);
    }
    else
    {
        viewSize = sf::Vector2f(zoomLevel * aspect, zoomLevel);
    }

    return sf::View(position, viewSize);
}