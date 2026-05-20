#pragma once 
#include "Object.h"
#include "Animation.h"

class Coin :
    public Object
{
public: 
    virtual void Begin() override;
    virtual void Update(float deltaTime) override;
    virtual void Render(Renderer& renderer) override;
private: 
    Animation animation;
};