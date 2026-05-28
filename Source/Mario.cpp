#include "Mario.h"
#include "Resources.h"
#include "Physics.h"
#include "Object.h"
#include "Game.h"
#include "Enemy.h"
#include <iostream>
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
    jumpSound.setVolume(30);

    fixtureData.listener = this;
    fixtureData.mario = this;
    fixtureData.type = FixtureDataType::Mario;

    b2BodyDef bodyDef{};
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = true;
    body = Physics::world.CreateBody(&bodyDef);

    b2FixtureDef fixtureDef{};
    fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
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
    fixtureDef.isSensor = true;
    groundFixture = body->CreateFixture(&fixtureDef);

    
}

void Mario::Update(float deltaTime)
{
    if(dead)
    {
        return;
    }

    if(pendingRespawn)
    {
        body->SetTransform(b2Vec2(3.0f, 3.0f), 0.0f);
        body->SetLinearVelocity(b2Vec2_zero);

        pendingRespawn = false;
    }
    if(invincible)
    {
    invincibleTimer -= deltaTime;

    if(invincibleTimer <= 0.0f)
    {
        invincible = false;
    }
    }

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

void Mario::OnBeginContact(b2Fixture* self,b2Fixture* other)
{
    FixtureData* data = (FixtureData*)other->GetUserData().pointer;

    if(!data)
    {
        return;
    }

    if(groundFixture == self && data->type == FixtureDataType::MapTile)
    {
        onGround++;
    }
    else if (data->type == FixtureDataType::Object &&
         data->object->tag == "coin")
        {
            // evitar contar dos veces
            if(!data->object->destroy)
            {
                data->object->destroy = true;

                coins++;

                std::cout << "coins = " << coins << "\n";
            }
        }
    else if (data->type == FixtureDataType::Object &&
    data->object->tag == "enemy")
    {
        Enemy* enemy = dynamic_cast<Enemy*>(data->object);

        if(enemy)
        {
            // si fue con el sensor de pies -> matar enemigo
            if(self == groundFixture)
            {
                enemy->Die();

                // rebote clásico Mario
                b2Vec2 velocity = body->GetLinearVelocity();
                velocity.y = -7.0f;
                body->SetLinearVelocity(velocity);
            }
            else
            {
                // daño a Mario
                LoseLife();
            }
        }
    }
}

void Mario::OnEndContact(b2Fixture* self, b2Fixture* other)
{
    FixtureData* data = (FixtureData*)other->GetUserData().pointer;

    if(groundFixture == self && data->type == FixtureDataType::MapTile && onGround>0)
    {    
        onGround--;
    }
   
}

size_t Mario::GetCoins()
{
    return coins;
}
int Mario::GetLives()
{
    return lives;
}

bool Mario::IsDead()
{
    return dead;
}

void Mario::LoseLife()
{
    if(invincible)
    {
        return;
    }
    if(dead)
    {
        return;
    }

    lives--;
    invincible = true;
    invincibleTimer = 2.0f;

    std::cout << "Lives: " << lives << "\n";

    pendingRespawn = true;

    if(lives <= 0)
    {
        dead = true;
    }
}

void Mario::Reset()
{
    if(body)
    {
        Physics::world.DestroyBody(body);
        body = nullptr;
    }

    coins = 0;
    lives = 3;

    dead = false;

    pendingRespawn = false;

    invincible = false;
    invincibleTimer = 0.0f;

    onGround = 0;

    facingLeft = false;

    Begin();
}