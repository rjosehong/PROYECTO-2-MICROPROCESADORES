#include "Instructions.h"
// =====================================
// PANTALLA DE INSTRUCCIONES
// =====================================
// Cargar recursos visuales.
Instructions::Instructions(float width, float height) {
    if(!font.loadFromFile("./resource/fonts/NintendoDS.ttf")) {
        cout << "No hay font\n";
    }
    
    // Cargar imagen tecla A.
    if(!aTexture.loadFromFile("./resource/textures/a_key.png")) {
        cout << "No está la imagen de a_key\n";
    }
    // Cargar imagen tecla D.
    if(!dTexture.loadFromFile("./resource/textures/d_key.png")) {
        cout << "No está la imagen de d_key\n";
    }
    // Cargar imagen tecla SPACE.
    if(!spaceTexture.loadFromFile("./resource/textures/space_key.png")) {
        cout << "No está la imagen de space_key\n";
    }

    aSprite.setTexture(aTexture);
    dSprite.setTexture(dTexture);
    spaceSprite.setTexture(spaceTexture);
    float targetwidth = 64.f;
    float targetheight = 64.f;

    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(50);
    text.setString(
        "To play you will need only\n"
        "three keys:\n"
        "A, D & SPACE"
    );
    text.setPosition(120, 40);

    text2.setFont(font);
    text2.setFillColor(sf::Color::White);
    text2.setCharacterSize(50);
    text2.setString(
        "Use the A key to move left\n"
        "and the D key to move right"
    );
    text2.setPosition(120, 320);

    aSprite.setPosition(250, 520);
    aSprite.setScale(0.4f, 0.4f);

    dSprite.setPosition(500, 520);
    dSprite.setScale(0.4f, 0.4f);

    text3.setFont(font);
    text3.setFillColor(sf::Color::White);
    text3.setCharacterSize(50);
    text3.setString(
        "Press the SPACE key\n"
        "to jump"
    );
    text3.setPosition(120, 650);

    spaceSprite.setPosition(650, 700);
    spaceSprite.setScale(0.5f, 0.5f);

    aSprite.setScale(64.f / aSprite.getLocalBounds().width, 64.f / aSprite.getLocalBounds().height);
    dSprite.setScale(64.f / dSprite.getLocalBounds().width,64.f / dSprite.getLocalBounds().height);
    spaceSprite.setScale(64.f / spaceSprite.getLocalBounds().width,64.f / spaceSprite.getLocalBounds().height);
}

Instructions::~Instructions() {
}

void Instructions::draw(sf::RenderWindow& window) {
    window.draw(text);
    window.draw(text2);
    window.draw(text3);
    window.draw(aSprite);
    window.draw(dSprite);
    window.draw(spaceSprite);
}
