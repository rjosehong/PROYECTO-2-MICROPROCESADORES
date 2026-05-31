#pragma once

#include "Animation.h"
#include "Object.h"
#include "Physics.h"

/// Goomba enemigo del juego.
class Enemy :
    public Object
{
public:

    virtual void Begin() override;
    virtual void Update(float deltaTime) override;
    virtual void Render(Renderer& renderer) override;

    /// Marca enemigo como muerto.
    void Die();

    /// Elimina el body físico.
    void DestroyPhysics();

private:

    /// Animación de caminar.
    Animation animation{};

    /// Velocidad horizontal.
    float movement = 3.0f;

    /// Tiempo desde que murió.
    float destroyTimer = 0.0f;

    /// Estado de muerte.
    bool isDead = false;

    /// Información Box2D.
    FixtureData fixtureData{};

    /// Body físico.
    b2Body* body{};
};