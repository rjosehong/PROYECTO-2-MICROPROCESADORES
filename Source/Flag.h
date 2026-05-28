#pragma once

#include "Object.h"
#include "Physics.h"

class Flag :
    public Object
{
public:
    virtual void Begin() override;
    virtual void Render(Renderer& renderer) override;

    void DestroyPhysics();

private:
    FixtureData fixtureData{};
    b2Body* body{};
};