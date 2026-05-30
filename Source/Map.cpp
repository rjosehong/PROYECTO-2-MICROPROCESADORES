#include "Map.h"
#include "Resources.h"
#include "Enemy.h"
#include <box2d/b2_body.h>
#include "Physics.h"
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include "Object.h"
#include "Coin.h"
#include "Flag.h"

Map::Map(float cellSize)
    : cellSize(cellSize), grid()
{
}



void Map::CreateCheckerboard(size_t width, size_t height)
{
    grid = std::vector(width, std::vector(height, (sf::Texture*)nullptr));

    bool last = 0;
    for (auto& column : grid)
    {
        for(auto& cell : column)
        {
            last = !last;
            if (last)
            {
                cell = &Resources::textures["block.png"];
            }
        }
        if(width % 2 == 0)
        {
            last = !last;
        }
    }
}

sf::Vector2f Map::CreateFromImage(const sf::Image& image, std::vector<Object*>& objects)
{
    objects.clear();
    grid.clear();
    grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 
        (sf::Texture*)nullptr));


    sf::Vector2f marioPosition{};
    sf::Color brown(185, 122, 87);
    sf::Color orange(255, 127, 39);
    sf::Color green(34, 177, 76);
    sf::Color pink(239, 136, 190);
    sf::Color gray(128, 128, 128);
    sf::Color flagPurple(57, 16, 123);
    sf::Color coinYellow(255, 242, 0);

    for(size_t x = 0; x < grid.size(); x++){
        for(size_t y = 0; y < grid[x].size(); y++)
        {
            sf::Color color = image.getPixel(x,y);
            Object* object = nullptr;
            if(color == sf::Color::Red)
            {
                marioPosition = sf::Vector2f(
                    cellSize * x + cellSize / 2.0f,
                    cellSize * y + cellSize / 2.0f
                );

                continue;
            }

            // bloque normal
            else if (color == sf::Color::Black)
            {
                grid[x][y] = &Resources::textures["block.png"];
            }

            // ladrillo
            else if (color == brown)
            {
                grid[x][y] = &Resources::textures["ladrillo.png"];
            }

            // bloque pregunta
            else if (color == orange)
            {
                grid[x][y] = &Resources::textures["bamarillo.jpeg"];
            }

            // tubo
            else if (color == green)
            {
                grid[x][y] = &Resources::textures["tubo.png"];
            }

            // castillo
            else if (color == pink)
            {
                grid[x][y] = &Resources::textures["castillo.png"];
            }

            // moneda
            else if (color == coinYellow)
            {
                object = new Coin();
            }

            // goomba
            else if (color == gray)
            {
                object = new Enemy();
            }

            // bandera
            else if (color == flagPurple)
            {
                object = new Flag();
            }
            
            if(object)
            {
                object->position = sf::Vector2f(cellSize * x + cellSize / 2.0f,
                         cellSize * y + cellSize / 2.0f);
                objects.push_back(object);
            }
            else if (grid[x][y])
            {
                b2BodyDef bodyDef{};

                // tamaño normal
                float halfWidth = cellSize / 2.0f;
                float halfHeight = cellSize / 2.0f;

                // posición normal
                float bodyX = cellSize * x + cellSize / 2.0f;
                float bodyY = cellSize * y + cellSize / 2.0f;

                // =========================
                // TUBOS
                // =========================

                if(grid[x][y] == &Resources::textures["tubo.png"])
                {
                    halfWidth = 1.0f;
                    halfHeight = 2.5f;

                    bodyX = cellSize * x + 0.5f;
                    bodyY = cellSize * y - 1.2f;
                }

                // =========================
                // CASTILLO
                // =========================

                if(grid[x][y] == &Resources::textures["castillo.png"])
                {
                    halfWidth = 2.5f;
                    halfHeight = 3.0f;

                    bodyX = cellSize * x + 2.5f;
                    bodyY = cellSize * y - 2.0f;
                }

                // aplicar posición FINAL
                bodyDef.position.Set(bodyX, bodyY);

                b2Body* body = Physics::world.CreateBody(&bodyDef);

                b2PolygonShape shape{};
                shape.SetAsBox(halfWidth, halfHeight);

                FixtureData* fixtureData = new FixtureData();
                fixtureData->type = FixtureDataType::MapTile;
                fixtureData->mapX = x;
                fixtureData->mapY = y;

                b2FixtureDef fixtureDef{};
                fixtureDef.userData.pointer = (uintptr_t)fixtureData;
                fixtureDef.density = 0.0f;
                fixtureDef.shape = &shape;
                body->CreateFixture(&fixtureDef);
            }
        }
    }

    return marioPosition;
}

void Map::Draw(Renderer& renderer)
{
    int x = 0;

    for(const auto& column : grid)
    {
        int y = 0;

        for(const auto& cell : column)
        {
            if(cell)
            {
                sf::Vector2f drawSize(cellSize, cellSize);

                sf::Vector2f drawPosition(
                    cellSize * x + cellSize / 2.0f,
                    cellSize * y + cellSize / 2.0f
                );

                // =========================
                // TUBO
                // =========================

                if(cell == &Resources::textures["tubo.png"])
                {
                    drawSize = sf::Vector2f(2.0f, 5.0f);

                    drawPosition.y -= 1.8f;
                }

                // =========================
                // CASTILLO
                // =========================

                if(cell == &Resources::textures["castillo.png"])
                {
                    drawSize = sf::Vector2f(5.0f, 5.0f);

                    drawPosition.y -= 2.0f;
                }

                renderer.Draw(
                    *cell,
                    drawPosition,
                    drawSize
                );
            }

            y++;
        }

        x++;
    }
}