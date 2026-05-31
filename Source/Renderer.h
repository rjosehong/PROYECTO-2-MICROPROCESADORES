#pragma once

#include <SFML/Graphics.hpp>

/// Encapsula las operaciones de dibujo.
class Renderer
{
public:

    Renderer(sf::RenderTarget& target);

    /// Dibuja una textura.
    void Draw(
        const sf::Texture& texture,
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        float angle = 0.0f
    );

    /// Destino de render.
    sf::RenderTarget& target;

private:

    /// Sprite reutilizable.
    sf::Sprite sprite{};
};