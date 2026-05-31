#include "Renderer.h"
#include <vector>

/// Constructor.
Renderer::Renderer(sf::RenderTarget& target)
    : target(target)
{
}

/// Dibuja una textura escalada y rotada.
void Renderer::Draw(
    const sf::Texture& texture,
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    float angle)
{
    // Asigna textura.
    sprite.setTexture(texture, true);

    // Centra origen.
    sprite.setOrigin(
        (sf::Vector2f)texture.getSize() / 2.0f
    );

    // Posiciona sprite.
    sprite.setPosition(position);

    // Escala sprite.
    sprite.setScale(
        sf::Vector2f(
            size.x / texture.getSize().x,
            size.y / texture.getSize().y
        )
    );

    // Rotación.
    sprite.setRotation(angle);

    // Render.
    target.draw(sprite);
}