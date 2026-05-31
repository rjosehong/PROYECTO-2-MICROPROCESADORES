// =====================================
// MAIN.CPP
// =====================================<
// Punto de entrada principal.
// Este archivo controla:
// - ventana
// - menú principal
// - estados del juego
// - entrada de usuario
// - ciclo principal
// - renderizado
// - actualización
// =====================================
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>

#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Menu.h"
#include "Instructions.h"
#include "Mario.h"

// Mutex utilizado para proteger
// acceso concurrente entre Update y Render.
std::mutex gameMutex;

/// Estados disponibles del juego.
enum class GameState
{
    MENU,
    PLAYING,
    INSTRUCTIONS,
    SCORES
};

// =====================================
// FUNCIÓN PRINCIPAL
// =====================================
int main()
{
    // Crear ventana principal.
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Mario Bros");
    // Limitar FPS para evitar consumo excesivo.
    window.setFramerateLimit(60);
    // Reloj para cálculo de deltaTime.
    sf::Clock deltaClock;
    // Sistema de renderizado.
    Renderer renderer(window);
    MainMenu menu(window.getSize().x, window.getSize().y);
    Instructions instructions(window.getSize().x, window.getSize().y);
    GameState state = GameState::MENU;
    Begin(window);

    // =====================================
    // GAME LOOP
    // =====================================
    while (window.isOpen())
    {
        // Tiempo transcurrido desde el último frame.
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event{};

        // Procesamiento de eventos.
        while (window.pollEvent(event))
        {
            // Cierre de aplicación.
            if (event.type == sf::Event::Closed)
                window.close();

            // =====================================
            // MENÚ PRINCIPAL
            // =====================================    
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
                        if (selected == 1) {
                            state = GameState::INSTRUCTIONS;
                        }
                        if (selected == 2) {
                            state = GameState::SCORES;
                        }
                        if (selected == 3) {
                            RestartGame(window);
                            state = GameState::PLAYING;
                        }

                        if (selected == 4) {
                            window.close();
                        }
                    }
                }
            }
            //Partida Activa
            else if (state == GameState::PLAYING)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape && !IsGameOver())
                    {
                        // Volver al menú principal.
                        state = GameState::MENU;
                    }

                    // Reinicio tras derrota o victoria.
                    if(IsGameOver() || HasPlayerWon())
                    {
                        if(event.key.code == sf::Keyboard::Enter)
                        {
                            // Reiniciar nivel actual.
                            RestartGame(window);
                        }
                        
                    }
                }
            }
            //Pantalla de Instrucciones
            else if (state == GameState::INSTRUCTIONS)
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape) {

                    // Regresar al menú.
                    state = GameState::MENU;
                }
            }

            //Pantalla de Scores
            else if (state == GameState::SCORES)
            {
                if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Escape) {

                    state = GameState::MENU;
                }
            }

        }
        // Limpiar frame actual.
        window.clear();

        if (state == GameState::MENU)
        {
            window.setView(window.getDefaultView());
            // Dibujar menú principal.
            menu.draw(window);
        }
        else if (state == GameState::PLAYING)
        {
            // Ejecutar actualización del juego.
            std::thread updateThread([&]()
            {
                std::lock_guard<std::mutex> lock(gameMutex);
                Update(deltaTime);
            });
            updateThread.join();
            // Configurar cámara del juego.
            window.setView(camera.GetView(window.getSize()));
            {
                std::lock_guard<std::mutex> lock(gameMutex);
                // Dibujar mundo completo.
                Render(renderer);
            }
        }
        else if (state == GameState::INSTRUCTIONS)
        {
            window.setView(window.getDefaultView());
            instructions.draw(window);
        }
        if(state == GameState::PLAYING)
        {
            window.setView(camera.GetUIView());
            RenderUI(renderer);
        }

        window.display();
    }
    return 0;
}