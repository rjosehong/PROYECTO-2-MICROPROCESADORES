#include "Game.h"
#include "Resources.h"
#include "Map.h"
#include "Mario.h"
#include <filesystem>
#include "Physics.h"
#include <SFML/Audio.hpp>
#include "Object.h"
#include "Coin.h"

Map map(1.0f);
Camera camera (20.0f);
Mario mario;
std::vector<Object*> objects{};

sf::Music music{};


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
    music.setVolume(50);

    Physics::Init();

    sf::Image image{};
    image.loadFromFile("./resource/textures/map (1).png");
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
    Physics::Update(deltaTime);
    mario.Update(deltaTime);
    camera.position = mario.position;
    for(auto& object : objects)
    {
        object->Update(deltaTime);
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