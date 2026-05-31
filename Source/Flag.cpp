#include "Flag.h"
#include "Resources.h"

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>

/// Inicialización de la bandera.
void Flag::Begin()
{
    tag = "flag";

    fixtureData.object = this;
    fixtureData.type = FixtureDataType::Object;

    b2BodyDef bodyDef{};
    bodyDef.position.Set(position.x, position.y);

    body = Physics::world.CreateBody(&bodyDef);

    // Sensor vertical.
    b2PolygonShape shape{};
    shape.SetAsBox(0.5f, 5.0f);

    b2FixtureDef fixtureDef{};
    fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
    fixtureDef.shape = &shape;
    fixtureDef.isSensor = true;

    body->CreateFixture(&fixtureDef);
}

/// Dibujo de la bandera.
void Flag::Render(Renderer& renderer)
{
    renderer.Draw(
        Resources::textures["bandera.png"],

        // Se desplaza hacia arriba para alinearse con el poste.
        sf::Vector2f(
            position.x,
            position.y - 4.5f
        ),

        sf::Vector2f(
            1.5f,
            10.0f
        )
    );
}

/// Destruye el body físico.
void Flag::DestroyPhysics()
{
    if(body)
    {
        Physics::world.DestroyBody(body);
        body = nullptr;
    }
}