#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>

#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Menu.h"

std::mutex gameMutex;

enum class GameState
{
    MENU,
    PLAYING
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Mario Bros");
    window.setFramerateLimit(60);
    sf::Clock deltaClock;
    Renderer renderer(window);
    MainMenu menu(window.getSize().x, window.getSize().y);
    GameState state = GameState::MENU;
    Begin(window);

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event{};

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameState::MENU) {
                if (event.type == sf::Event::KeyReleased) {

                    if (event.key.code == sf::Keyboard::Up) {
                        menu.MoveUp();
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        menu.MoveDown();
                    }
                    if (event.key.code == sf::Keyboard::Enter) {

                        int selected = menu.MainMenuPressed();

                        if (selected == 0) {
                            state = GameState::PLAYING;
                        }
                        if (selected == 3) {
                            window.close();
                        }
                    }
                }
            }
            else if (state == GameState::PLAYING)
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape) {

                    state = GameState::MENU;
                }
            }
        }
        window.clear();

        if (state == GameState::MENU)
        {
            window.setView(window.getDefaultView());
            menu.draw(window);
        }
        else if (state == GameState::PLAYING)
        {
            std::thread updateThread([&]()
            {
                std::lock_guard<std::mutex> lock(gameMutex);
                Update(deltaTime);
            });
            updateThread.join();
            window.setView(camera.GetView(window.getSize()));
            {
                std::lock_guard<std::mutex> lock(gameMutex);
                Render(renderer);
            }
        }
        window.display();
    }
    return 0;
}