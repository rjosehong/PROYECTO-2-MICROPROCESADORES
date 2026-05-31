#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include <box2d/b2_body.h>
#include "Physics.h"
#include "Animation.h"
#include <SFML/Audio.hpp>
#include "Object.h"
#include <box2d/b2_fixture.h>

/// Clase principal del jugador.
/// Gestiona movimiento, físicas, colisiones,
/// vidas, monedas, sonidos y estados de victoria/derrota.
class Mario
    : public ContactListener
{
public: 
    /// Indica si el jugador alcanzó la bandera.
    bool HasWon();
    /// Devuelve las vidas actuales del jugador.
    int GetLives();
    /// Reduce una vida al jugador.
    /// También activa invencibilidad temporal
    /// para evitar múltiples daños consecutivos.
    void LoseLife();
    /// Indica si el jugador perdió todas sus vidas.
    bool IsDead();
    /// Reinicia completamente el estado del jugador
    /// para comenzar nuevamente el nivel.
    void Reset();
    /// Inicializa físicas, sonidos,
    /// animaciones y colisiones de Mario.
    void Begin();
    /// Actualización principal del jugador.
    // Maneja movimiento, respawn,
    /// invencibilidad y animaciones.
    void Update(float deltaTime);
    /// Renderiza el sprite actual de Mario.
    void Draw(Renderer& renderer);
    /// Se ejecuta cuando Mario entra en contacto
    /// con otro fixture de Box2D.
    virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
    /// Se ejecuta cuando Mario deja de tocar
    /// otro fixture de Box2D.
    virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;

    /// Devuelve la cantidad de monedas recolectadas.
    size_t GetCoins();

    int lives = 3;
    bool dead = false;
    bool won = false;
    bool pendingWin = false;
    bool pendingRespawn = false;

    float invincibleTimer = 0.0f;
    bool invincible = false;

    sf::Vector2f position{};
    sf::Vector2f spawnPosition{};
    float angle{};
private:

    Animation runAnimation{};
    sf::Texture textureToDraw{};
    
    sf::Sound jumpSound{};
    sf::Sound coinSound{};
    sf::Sound stompSound{};
    sf::Sound winSound{};
    sf::Sound loseSound{};

    FixtureData fixtureData{};
    b2Body* body{};
    b2Fixture* groundFixture;
    
    size_t onGround = 0;
    bool facingLeft = false;

    size_t coins{};
};