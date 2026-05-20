#include "Coin.h"
#include "Resources.h"

void Coin::Begin()
{
    animation =Animation(1.4f,
    {
        AnimFrame(1.3f, Resources::textures["coin14.png"]),
        AnimFrame(1.2f, Resources::textures["coin13.png"]),
        AnimFrame(1.1f, Resources::textures["coin12.png"]),
        AnimFrame(1.0f, Resources::textures["coin11.png"]),
        AnimFrame(0.9f, Resources::textures["coin10.png"]),
        AnimFrame(0.8f, Resources::textures["coin9.png"]),
        AnimFrame(0.7f, Resources::textures["coin8.png"]),
        AnimFrame(0.6f, Resources::textures["coin7.png"]),
        AnimFrame(0.5f, Resources::textures["coin6.png"]),
        AnimFrame(0.4f, Resources::textures["coin5.png"]),
        AnimFrame(0.3f, Resources::textures["coin4.png"]),
        AnimFrame(0.2f, Resources::textures["coin3.png"]),
        AnimFrame(0.1f, Resources::textures["coin2.png"]),
        AnimFrame(0.0f, Resources::textures["coin1.png"]),
    });
}
void Coin::Update(float deltaTime)
{
    animation.Update(deltaTime);
}
void Coin::Render(Renderer& renderer)
{
    renderer.Draw(animation.GetTexture(), position, sf::Vector2f(0.8f, 0.8f));
}