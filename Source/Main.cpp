#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>

#include "Game.h"
#include "Camera.h"
#include "Renderer.h"

std::mutex gameMutex;

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200,900), "Mario Bros");
    sf::Clock deltaClock;
    Renderer renderer(window);

    window.setFramerateLimit(60);

    Begin(window);

    while(window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::thread updateThread([&]()
        {
            std::lock_guard<std::mutex> lock(gameMutex);
            Update(deltaTime);
        });

        updateThread.join();

        window.setView(camera.GetView(window.getSize()));

        window.clear(sf::Color(20,20,20));

        {
            std::lock_guard<std::mutex> lock(gameMutex);
            Render(renderer);
        }

        window.display();
    }

    return 0;
}