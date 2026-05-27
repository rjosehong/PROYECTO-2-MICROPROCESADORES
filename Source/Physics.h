#pragma once
#include <box2d/b2_body.h>
#include "Renderer.h"

class MyDebugDraw;
class Mario;
class Object;

class ContactListener
{
public:
    virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;
    virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;
};

enum class FixtureDataType
{
    Mario,
    MapTile,
    Object
};

struct FixtureData
{
    FixtureDataType type;
    ContactListener* listener;
    
    union 
    {
        Mario* mario;
        Object* object;
        struct { int mapX, mapY;};
    };
    
};

class Physics
{
public:
    static void Init();
    static void Update(float deltaTime);
    static void DebugDraw(Renderer& renderer);

    static b2World world;
    static MyDebugDraw* debugDraw;
}; 