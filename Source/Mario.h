#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include <box2d/b2_body.h>
#include "Physics.h"
#include "Animation.h"
#include <SFML/Audio.hpp>
#include "Object.h"
#include <box2d/b2_fixture.h>

class Mario
    : public ContactListener
{
public: 

    bool HasWon();
    int GetLives();
    void LoseLife();
    bool IsDead();
    void Reset();

    void Begin();
    void Update(float deltaTime);
    void Draw(Renderer& renderer);

    virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
    virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;

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