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
// =========================
// INICIALIZACIÓN DE MARIO
// =========================
void Mario::Begin()
{
    // Configuración de animación de carrera.
    runAnimation = Animation(0.45f,
    {
        AnimFrame(0.30f, Resources::textures["run3.png"]),
        AnimFrame(0.15f, Resources::textures["run2.png"]),
        AnimFrame(0.0f, Resources::textures["run1.png"]),
    });

    // Carga de efectos de sonido.
    jumpSound.setBuffer(Resources::sounds["jump.wav"]);
    jumpSound.setVolume(30);

    coinSound.setBuffer(Resources::sounds["coin.wav"]);
    coinSound.setVolume(40);

    stompSound.setBuffer(Resources::sounds["stomp.wav"]);
    stompSound.setVolume(40);

    winSound.setBuffer(Resources::sounds["sclear.wav"]);
    winSound.setVolume(50);

    loseSound.setBuffer(Resources::sounds["gover.wav"]);
    loseSound.setVolume(50);

    // Información utilizada por Box2D
    // para identificar a Mario durante colisiones.
    fixtureData.listener = this;
    fixtureData.mario = this;
    fixtureData.type = FixtureDataType::Mario;

    // Creación del cuerpo físico principal.
    b2BodyDef bodyDef{};
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = true;
    body = Physics::world.CreateBody(&bodyDef);

    b2FixtureDef fixtureDef{};
    fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    // Colisiones redondeadas para mejorar
    // el movimiento sobre superficies.
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

    // Sensor inferior utilizado para detectar
    // cuándo Mario está tocando el suelo.
    polygonShape.SetAsBox(0.4f, 0.2f, b2Vec2(0.0f, 1.0f), 0.0f);
    fixtureDef.isSensor = true;
    groundFixture = body->CreateFixture(&fixtureDef);

    
}

// =========================
// ACTUALIZACIÓN DE MARIO
// =========================
void Mario::Update(float deltaTime)
{
    // No actualizar si el jugador ya perdió.
    if(dead)
    {
        return;
    }

    // Procesar victoria pendiente.
    if(pendingWin)
    {
        won = true;
        pendingWin = false;

        std::cout << "YOU WIN\n";

        return;
    }

    // Reubicar a Mario en el punto de respawn.
    if(pendingRespawn)
    {
        body->SetTransform(
            b2Vec2(spawnPosition.x, spawnPosition.y),
            0.0f
        );
        body->SetLinearVelocity(b2Vec2_zero);

        pendingRespawn = false;
    }

    // Control de invencibilidad temporal.
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

    // Lectura de entrada del jugador.
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
        textureToDraw = Resources::textures["BMario.png"];
    }

    if(!onGround)
    {
        textureToDraw = Resources::textures["jump.png"];
    }

    // Aplicar velocidad calculada al cuerpo físico.
    body->SetLinearVelocity(velocity);

    // Sincronizar posición lógica con Box2D.
    position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
    angle = body->GetAngle() *(180.0f /PI);
     
    
}

void Mario::Draw(Renderer& renderer)
{
    renderer.Draw(textureToDraw, position, 
        sf::Vector2f(facingLeft ? -1.0f : 1.0f, 2.0f), angle);
}

// =========================
// DETECCIÓN DE COLISIONES
// =========================
void Mario::OnBeginContact(b2Fixture* self,b2Fixture* other)
{
    FixtureData* data = (FixtureData*)other->GetUserData().pointer;

    if(!data)
    {
        return;
    }

    // Detección de suelo.
    if(groundFixture == self && data->type == FixtureDataType::MapTile)
    {
        onGround++;
    }

    // Recolección de monedas.
    else if (data->type == FixtureDataType::Object &&
         data->object->tag == "coin")
        {
            // evitar contar dos veces
            if(!data->object->destroy)
            {
                data->object->destroy = true;

                coins++;
                coinSound.play();

                std::cout << "coins = " << coins << "\n";
            }
        }

    // Interacción con enemigos.    
    else if (data->type == FixtureDataType::Object &&
    data->object->tag == "enemy")
    {
        Enemy* enemy = dynamic_cast<Enemy*>(data->object);

        if(enemy)
        {
            // si fue con el sensor de pies -> matar enemigo
            if(self == groundFixture)
            {
                // Mario aplasta al enemigo desde arriba.
                enemy->Die();
                stompSound.play();

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

    // Activación de victoria.
   else if (data->type == FixtureDataType::Object &&
         data->object->tag == "flag")
    {
        // evitar múltiples triggers
        if(!pendingWin && !won)
        {
            pendingWin = true;
            winSound.play();
        }
    }
}

// =========================
// FIN DE COLISIÓN
// =========================
void Mario::OnEndContact(b2Fixture* self, b2Fixture* other)
{
    FixtureData* data = (FixtureData*)other->GetUserData().pointer;

    if(groundFixture == self && data->type == FixtureDataType::MapTile && onGround>0)
    {   // Mario deja de tocar una superficie. 
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

bool Mario::HasWon()
{
    return won;
}

// =========================
// PÉRDIDA DE VIDA
// =========================
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
    // Evitar daño múltiple inmediato.
    invincible = true;
    invincibleTimer = 2.0f;

    std::cout << "Lives: " << lives << "\n";
    // Programar respawn seguro.
    pendingRespawn = true;

    if(lives <= 0)
    {
        // Activar estado de Game Over.
        dead = true;
        loseSound.play();
    }
}

// =========================
// REINICIO DE MARIO
// =========================
void Mario::Reset()
{
    // Restaurar estado inicial del jugador.
    coins = 0;
    lives = 3;

    dead = false;
    won = false;

    pendingWin = false;

    // usar respawn seguro
    pendingRespawn = true;

    invincible = false;
    invincibleTimer = 0.0f;

    onGround = 0;

    facingLeft = false;
}