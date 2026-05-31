#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

/// Pantalla que explica los controles.
class Instructions {
    /// Inicializa imágenes y textos.
    public:
    Instructions(float width, float height);
    ~Instructions();
    /// Dibuja la pantalla de instrucciones.
    void draw(RenderWindow& window);

    private:
        sf::Font font;

        sf::Texture aTexture;
        sf::Texture dTexture;
        sf::Texture spaceTexture;

        sf::Sprite aSprite;
        sf::Sprite dSprite;
        sf::Sprite spaceSprite;

        sf::Text text;
        sf::Text text2;
        sf::Text text3;
};