#include "Mario.h"
#include "Resources.h"
#include "Physics.h"
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_circle_shape.h>

constexpr float PI = 22.0f / 7.0f;

const float movementSpeed = 7.0f;
const float jumpVelocity = 11.0f;

void Mario::Begin()
{
    runAnimation = Animation(0.45f,
    {
        AnimFrame(0.30f, Resources::textures["run3.png"]),
        AnimFrame(0.15f, Resources::textures["run2.png"]),
        AnimFrame(0.0f, Resources::textures["run1 (2).png"]),
    });

    jumpSound.setBuffer(Resources::sounds["jump.wav"]);

    b2BodyDef bodyDef{};
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = true;
    body = Physics::world.CreateBody(&bodyDef);

    b2FixtureDef fixtureDef{};
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    b2CircleShape circleShape{};
    circleShape.m_radius = 0.5f;
    circleShape.m_p.Set(0.0f, -0.5f);
    fixtureDef.shape = &circleShape;
    body->CreateFixture(&fixtureDef);

    circleShape.m_p.Set(0.0f, 0.5f);
    body->CreateFixture(&fixtureDef);

    b2PolygonShape polygonShape{};
    polygonShape.SetAsBox(0.5f, 0.5f);
    fixtureDef.shape = &polygonShape;
    body->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.4f, 0.2f, b2Vec2(0.0f, 1.0f), 0.0f);
    fixtureDef.userData.pointer = (uintptr_t)this;
    fixtureDef.isSensor = true;
    body->CreateFixture(&fixtureDef);

    
}

void Mario::Update(float deltaTime)
{
    float move = movementSpeed;

    runAnimation.Update(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        move *= 2;
    }
    b2Vec2 velocity = body->GetLinearVelocity();
    velocity.x = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        velocity.x -= move;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x += move;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && onGround)
    {
        velocity.y = -jumpVelocity;
        jumpSound.play();
        jumpSound.setVolume(30);
    }

    textureToDraw = runAnimation.GetTexture();

    if(velocity.x < -0.02f)
    {
        facingLeft = true;
    }
    else if (velocity.x > 0.02f)
    {
        facingLeft = false;
    }
    else 
    {
        textureToDraw = Resources::textures["bmario.png"];
    }

    if(!onGround)
    {
        textureToDraw = Resources::textures["jump.png"];
    }

    body->SetLinearVelocity(velocity);

    position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
    angle = body->GetAngle() *(180.0f /PI);
     
    
}

void Mario::Draw(Renderer& renderer)
{
    renderer.Draw(textureToDraw, position, 
        sf::Vector2f(facingLeft ? -1.0f : 1.0f, 2.0f), angle);
}

void Mario::OnBeginContact()
{
    onGround++;
}

void Mario::OnEndContact()
{
    if(onGround > 0)
    {
        onGround--;
    }
}