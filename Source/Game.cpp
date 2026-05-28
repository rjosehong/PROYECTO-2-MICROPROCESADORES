#include "Game.h"
#include "Resources.h"
#include "Map.h"
#include "Mario.h"
#include <filesystem>
#include "Physics.h"
#include <SFML/Audio.hpp>
#include "Object.h"
#include "Coin.h"
#include "Enemy.h"
#include <box2d/b2_world.h>

Map map(1.0f);
Camera camera (20.0f);
Mario mario{};
std::vector<Object*> objects{};

sf::Music music{};

sf::Font font{};
sf::Text coinsText("Monedas", font);
sf::Text livesText("Lives", font);

void Begin(const sf::Window& window)
{
    for(auto&file : std::filesystem::directory_iterator("./resource/textures/"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".png"
                || file.path().extension() == ".jpg"))
        {
            Resources::textures[file.path().filename().string()]
                .loadFromFile(file.path().string());
        }
    }

    for(auto&file : std::filesystem::directory_iterator("./resource/sounds/"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".ogg"
                || file.path().extension() == ".wav"))
        {
            Resources::sounds[file.path().filename().string()]
                .loadFromFile(file.path().string());
        }
    }

    

    music.openFromFile("./resource/sounds/music.ogg");
    music.setLoop(true);
    music.setVolume(35);

    font.loadFromFile("./resource/fonts/SuperMarioBros.ttf");
    coinsText.setFillColor(sf::Color::White);
    coinsText.setOutlineColor(sf::Color::Black);
    coinsText.setOutlineThickness(1.0f);
    coinsText.setScale(0.1f,0.1f);

    livesText.setFillColor(sf::Color::White);
    livesText.setOutlineColor(sf::Color::Black);
    livesText.setOutlineThickness(1.0f);
    livesText.setScale(0.1f,0.1f);

    Physics::Init();

    sf::Image image{};
    image.loadFromFile("./resource/textures/map.png");
    mario.position = map.CreateFromImage(image, objects);

    mario.Begin();
    for(auto& object : objects)
    {
        object->Begin();
    }

    music.play();
    

    
}

void Update(float deltaTime)
{
    if(mario.IsDead())
    {
        return;
    }

    Physics::Update(deltaTime);

    mario.Update(deltaTime);

    camera.position = mario.position;

    for(auto& object : objects)
    {
        object->Update(deltaTime);
    }

    for(auto it = objects.begin(); it != objects.end(); )
{
    Object* object = *it;

    if(object->destroy)
    {
        if(!object->physicsDestroyed)
        {
            if(Coin* coin = dynamic_cast<Coin*>(object))
            {
                coin->DestroyPhysics();
            }

            if(Enemy* enemy = dynamic_cast<Enemy*>(object))
            {
                enemy->DestroyPhysics();
            }

            object->physicsDestroyed = true;
        }

        delete object;
        it = objects.erase(it);
    }
    else
    {
        ++it;
    }
}
}

void Render (Renderer& renderer)
{

    renderer.Draw (Resources::textures["background.png"], camera.position, camera.GetViewSize());
    map.Draw(renderer);
    mario.Draw(renderer);

    for(auto& object : objects)
    {
        object->Render(renderer);
    }
    Physics::DebugDraw(renderer);
}

void RenderUI(Renderer& renderer)
{
    coinsText.setPosition(
        -camera.GetViewSize() / 2.0f + sf::Vector2f(2.0f, 1.0f)
    );

    coinsText.setString(
        "Coins: " + std::to_string(mario.GetCoins())
    );

    renderer.target.draw(coinsText);

    livesText.setPosition(
        -camera.GetViewSize() / 2.0f + sf::Vector2f(2.0f, 3.0f)
    );

    livesText.setString(
        "Lives: " + std::to_string(mario.GetLives())
    );

    renderer.target.draw(livesText);

    // GAME OVER
    if(mario.IsDead())
    {
        sf::Text gameOver("GAME OVER", font);

        gameOver.setFillColor(sf::Color::Red);
        gameOver.setOutlineColor(sf::Color::Black);
        gameOver.setOutlineThickness(2.0f);

        gameOver.setScale(0.14f, 0.14f);

        gameOver.setPosition(-6.0f, -2.0f);

        renderer.target.draw(gameOver);

        sf::Text restart("Press ENTER to Restart", font);

        restart.setFillColor(sf::Color::White);
        restart.setOutlineColor(sf::Color::Black);
        restart.setOutlineThickness(1.0f);

        restart.setScale(0.05f, 0.05f);

        restart.setPosition(-5.0f, 1.5f);

        renderer.target.draw(restart);
    }
}

void DeleteObject(Object* object)
{
    const auto& it = std::find(objects.begin(), objects.end(), object);
    if(it != objects.end())
    {
        delete *it;
        objects.erase(it);
    }
} 

void RestartGame(const sf::Window& window)
{
    // =========================
    // BORRAR OBJETOS
    // =========================

    for(Object* object : objects)
    {
        if(Coin* coin = dynamic_cast<Coin*>(object))
        {
            coin->DestroyPhysics();
        }

        if(Enemy* enemy = dynamic_cast<Enemy*>(object))
        {
            enemy->DestroyPhysics();
        }

        delete object;
    }

    objects.clear();

    // =========================
    // RECARGAR MAPA
    // =========================

    sf::Image image{};

    image.loadFromFile("./resource/textures/map.png");

    mario.position = map.CreateFromImage(image, objects);

    // =========================
    // RESET MARIO
    // =========================

    mario.Reset();

    // =========================
    // RECREAR OBJETOS
    // =========================

    for(auto& object : objects)
    {
        object->Begin();
    }
}

bool IsGameOver()
{
    return mario.IsDead();
}