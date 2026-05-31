#pragma once

#include "Renderer.h"

/// Clase base para todos los objetos del juego.
class Object
{
public:

    /// Se ejecuta al iniciar el objeto.
    virtual void Begin(){}

    /// Se ejecuta cada frame.
    virtual void Update(float deltaTime){}

    /// Dibuja el objeto.
    virtual void Render(Renderer& renderer){}

    /// Identificador lógico.
    std::string tag{};

    /// Posición en el mundo.
    sf::Vector2f position{};

    /// Rotación.
    float angle {};

    /// Marca de destrucción lógica.
    bool destroy = false;

    /// Indica si ya se destruyó su física.
    bool physicsDestroyed = false;
};