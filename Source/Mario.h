#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include <box2d/b2_body.h>
#include "Physics.h"
#include "Animation.h"
#include <SFML/Audio.hpp>
class Mario
    : public ContactListener
{
public: 
    void Begin();
    void Update(float deltaTime);
    void Draw(Renderer& renderer);

    virtual void OnBeginContact() override;
    virtual void OnEndContact() override;

    sf::Vector2f position{};
    float angle{};
private:
    Animation runAnimation{};
    sf::Texture textureToDraw{};
    sf::Sound jumpSound{};

    b2Body* body{};
    size_t onGround = 0;
    bool facingLeft = false;
};