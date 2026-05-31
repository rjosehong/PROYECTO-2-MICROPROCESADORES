#pragma once

#include "Object.h"
#include "Physics.h"

/// Bandera que indica victoria.
class Flag :
    public Object
{
public:

    /// Inicialización.
    virtual void Begin() override;

    /// Dibujo.
    virtual void Render(Renderer& renderer) override;

    /// Destruye body físico.
    void DestroyPhysics();

private:

    /// Información personalizada Box2D.
    FixtureData fixtureData{};

    /// Body físico.
    b2Body* body{};
};