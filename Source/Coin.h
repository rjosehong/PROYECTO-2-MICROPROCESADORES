#pragma once

#include "Object.h"
#include "Animation.h"
#include "Physics.h"
#include <box2d/b2_body.h>

/// Representa una moneda coleccionable.
class Coin :
    public Object
{
public:

    /// Destructor.
    ~Coin();

    /// Inicializa la moneda.
    virtual void Begin() override;

    /// Actualiza la animación.
    virtual void Update(float deltaTime) override;

    /// Dibuja la moneda.
    virtual void Render(Renderer& renderer) override;

    /// Destruye el body físico asociado.
    void DestroyPhysics();

private:

    /// Animación de rotación de la moneda.
    Animation animation;

    /// Body físico Box2D.
    b2Body* body;
};