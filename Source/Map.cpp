#include "Map.h"
#include "Resources.h"
#include <box2d/b2_body.h>
#include "Physics.h"
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include "Object.h"
#include "Coin.h"

Map::Map(float cellSize)
    : cellSize(cellSize), grid()
{
}

void Map::CreateCheckerboard(size_t width, size_t height)
{
    grid = std::vector(width, std::vector(height, 0));

    bool last = 0;
    for (auto& column : grid)
    {
        for(auto& cell : column)
        {
            last = cell = !last;
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
    grid = std::vector(image.getSize().x, std::vector(image.getSize().y, 0));

    sf::Vector2f marioPosition{};
    for(size_t x = 0; x < grid.size(); x++){
        for(size_t y = 0; y < grid[x].size(); y++)
        {
            sf::Color color = image.getPixel(x,y);
            int tileType = 0;
            bool isSolid = false;

            // 1. Suelo (Negro o Verde)
            if (color == sf::Color::Black || color == sf::Color::Green)
            {
                tileType = 1;
                isSolid = true;
            }        
            // 2. Mario (Rojo Puro)
            else if(color == sf::Color::Red)
            {
                marioPosition = sf::Vector2f(cellSize * x + cellSize / 2.0f,
                                             cellSize * y + cellSize / 2.0f);
            }
            // 3. Monedas (#FFF200 -> R:255, G:242, B:0)
            else if (color == sf::Color(255, 242, 0))
            {
                Object* coin = new Coin();
                coin->position = sf::Vector2f(cellSize * x + cellSize / 2.0f,
                                              cellSize * y + cellSize / 2.0f);
                objects.push_back(coin);
            }
            // 4. Ladrillo Café (#B97A57 -> R:185, G:122, B:87)
            else if(color == sf::Color(185, 122, 87))
            {
                tileType = 2;
                isSolid = true;
            }
            // 5. Bloque Pregunta (#FF7F27 -> R:255, G:127, B:39)
            else if(color == sf::Color(255, 127, 39)) 
            {
                tileType = 3;
                isSolid = true;
            }
            // 6. Castillo Rosa (#EF88BE -> R:239, G:136, B:190)
            else if(color == sf::Color(239, 136, 190))
            {
                tileType = 4;
                isSolid = false; 
            }
            // 7. Goomba Gris (#808080 -> R:128, G:128, B:128)
            else if(color == sf::Color(128, 128, 128))
            {
                tileType = 5;
                isSolid = false; 
            }
            // 8. Tubo Verde (#22B14C -> R:34, G:177, B:76)
            else if(color == sf::Color(34, 177, 76))
            {
                tileType = 6;
                isSolid = true;
            }
            // 9. Bandera (#39107B -> R:57, G:16, B:123)
            else if(color == sf::Color(57, 16, 123))
            {
                tileType = 7;
                isSolid = false;
            }

            // Si detectamos un bloque que pertenece a la grilla, lo asignamos y creamos su física
            if (tileType > 0)
            {
                grid[x][y] = tileType;

                if (isSolid)
                {
                    b2BodyDef bodyDef{};
                    bodyDef.position.Set(cellSize * x + cellSize / 2.0f,
                                         cellSize * y + cellSize / 2.0f);
                    b2Body* body = Physics::world.CreateBody(&bodyDef);
                    b2PolygonShape shape{};
                    shape.SetAsBox(cellSize / 2.0f, cellSize / 2.0f);
                    body->CreateFixture(&shape, 0.0f);
                }
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
            if (cell > 0) 
            {
                std::string textureName = "";
                sf::Vector2f size(cellSize, cellSize);
                sf::Vector2f offset(0.0f, 0.0f);

                switch (cell)
                {
                    case 1: textureName = "block.png"; break;
                    case 2: textureName = "ladrillo.png"; break;
                    case 3: textureName = "bamarillo.png"; break; 
                    
                    case 4: // CASTILLO
                        textureName = "castillo.png";
                        size = sf::Vector2f(cellSize * 4.0f, cellSize * 4.0f); 
                        offset.x = cellSize * 1.0f;
                        offset.y = -cellSize * 1.5f;
                        break;

                    case 5: // GOOMBA
                        textureName = "Goomba.png";
                        break;

                    case 6: // TUBO
                        textureName = "tubo.png";
                        size = sf::Vector2f(cellSize * 1.5f, cellSize * 5.0f); 
                        offset.y = -cellSize * 1.75f; 
                        break;

                    case 7: // BANDERA
                        textureName = "bandera.png";
                        size = sf::Vector2f(cellSize * 1.2f, cellSize * 12.0f); 
                        offset.y = -cellSize * 5.5f; 
                        break;
                }

                if (!textureName.empty())
                {
                    renderer.Draw(Resources::textures[textureName], 
                        sf::Vector2f(cellSize * x + cellSize / 2.0f + offset.x,
                                     cellSize * y + cellSize / 2.0f + offset.y),
                        size);
                }
            }
            y++;
        }
        x++;
    }
}