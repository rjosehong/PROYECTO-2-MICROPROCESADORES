#include "Menu.h"

// =====================================
// MENÚ PRINCIPAL
// =====================================
// Crear todos los elementos visuales.
MainMenu::MainMenu(float width, float height) {
    // Cargar tipografía.
    if(!font.loadFromFile("./resource/fonts/SuperMarioBros.ttf")) {
        cout << "No hay font\n";
    }

    // Cargar imagen de fondo.
    if(!backgroundTexture.loadFromFile("./resource/textures/menubackground.png")) {
        std::cout << "No hay fondo para el menú\n";
    }

    backgroundSprite.setTexture(backgroundTexture);
    float scaleX = width / backgroundSprite.getLocalBounds().width;
    float scaleY = height / backgroundSprite.getLocalBounds().height;
    backgroundSprite.setScale(scaleX, scaleY);

    textTitle.setFont(font);
    textTitle.setFillColor(sf::Color::White);
    textTitle.setString("Mario");
    textTitle.setCharacterSize(70);
    textTitle.setPosition(420,60);
    textTitle.setOutlineColor(sf::Color::Black);
    textTitle.setOutlineThickness(5);

    textTitle2.setFont(font);
    textTitle2.setFillColor(sf::Color::White);
    textTitle2.setString("Bros");
    textTitle2.setCharacterSize(70);
    textTitle2.setPosition(420,140);
    textTitle2.setOutlineColor(sf::Color::Black);
    textTitle2.setOutlineThickness(5);

    //Play
    mainMenu[0].setFont(font);
    mainMenu[0].setFillColor(sf::Color::White);
    mainMenu[0].setString("Play");
    mainMenu[0].setCharacterSize(55);
    mainMenu[0].setPosition(420,300);
    
    //Instrucciones
    mainMenu[1].setFont(font);
    mainMenu[1].setFillColor(sf::Color::White);
    mainMenu[1].setString("Instructions");
    mainMenu[1].setCharacterSize(55);
    mainMenu[1].setPosition(420,380);

    //Pausa
    mainMenu[2].setFont(font);
    mainMenu[2].setFillColor(sf::Color::White);
    mainMenu[2].setString("Scores");
    mainMenu[2].setCharacterSize(55);
    mainMenu[2].setPosition(420,460);

    //Restart
    mainMenu[3].setFont(font);
    mainMenu[3].setFillColor(sf::Color::White);
    mainMenu[3].setString("Restart");
    mainMenu[3].setCharacterSize(55);
    mainMenu[3].setPosition(420,540);

    //Exit
    mainMenu[4].setFont(font);
    mainMenu[4].setFillColor(sf::Color::White);
    mainMenu[4].setString("Exit");
    mainMenu[4].setCharacterSize(55);
    mainMenu[4].setPosition(420,620);

    MainMenuSelected = -1;
}
MainMenu::~MainMenu() {
}

void MainMenu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite);
    window.draw(textTitle);
    window.draw(textTitle2);
    for(int i = 0; i < max_main_menu; i++) {
        window.draw(mainMenu[i]);
    }
}

void MainMenu::MoveUp() {
    mainMenu[MainMenuSelected].setFillColor(sf::Color::White);
    MainMenuSelected--;
    if (MainMenuSelected < 0) {
        MainMenuSelected = max_main_menu - 1;
    }
    mainMenu[MainMenuSelected].setFillColor(sf::Color::Blue);
}

void MainMenu::MoveDown() {
    mainMenu[MainMenuSelected].setFillColor(sf::Color::White);
    MainMenuSelected++;
    if (MainMenuSelected >= max_main_menu) {
        MainMenuSelected = 0;
    }
    mainMenu[MainMenuSelected].setFillColor(sf::Color::Red);
}