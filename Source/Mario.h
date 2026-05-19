#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include <box2d/b2_body.h>
#include "Physics.h"
#include "Animation.h"
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

    b2Body* body{};
    size_t onGround = 0;
    bool facingLeft = false;
};