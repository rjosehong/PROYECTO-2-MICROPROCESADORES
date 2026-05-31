#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

// Número total de opciones disponibles.
#define max_main_menu 5

/// Clase encargada de mostrar
/// y controlar el menú principal.
class MainMenu {
    /// Inicializa todos los elementos visuales.
    public:
        MainMenu(float width, float height);
        ~MainMenu();
        /// Dibuja el menú completo.
        void draw(RenderWindow& window);
        void MoveUp();
        void MoveDown();
        
        /// Devuelve la opción seleccionada.
        int MainMenuPressed() {
            return MainMenuSelected;
        }
    
    private:
        int MainMenuSelected;
        sf::Font font;
        sf::Text mainMenu[max_main_menu];
        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;
        sf::Text textTitle;
        sf::Text textTitle2;
};