// =====================================
// GAME.CPP
// =====================================
//
// Control principal del juego.
//
// Este archivo gestiona:
//
// - carga de recursos
// - carga del mapa
// - actualización global
// - renderizado
// - UI
// - reinicio de nivel
// - destrucción segura de objetos
//
// =====================================
#include "Game.h"
#include "Resources.h"
#include "Map.h"
#include "Mario.h"
#include <filesystem>
#include "Physics.h"
#include <SFML/Audio.hpp>
#include "Object.h"
#include "Coin.h"
#include "ScoreManager.h"
#include "ScoreThread.h"
#include "Enemy.h"
#include <box2d/b2_world.h>
#include "Flag.h"
#include "ScoreManager.h"
#include <mutex>
#include "EnemyThread.h"
#include "CoinStatsThread.h"

// Mapa actual del juego.
Map map(1.0f);
// Cámara principal.
Camera camera (20.0f);
// Instancia única del jugador.
Mario mario{};
//Lista global de objetos dinámicos
std::vector<Object*> objects{};
std::mutex objectsMutex;

// Música de fondo.
sf::Music music{};

// Fuente utilizada por la UI.
sf::Font font{};
// Texto para mostrar monedas.
sf::Text coinsText("Monedas", font);
// Texto para mostrar vidas.
sf::Text livesText("Lives", font);
/// Texto utilizado para mostrar el score actual.
sf::Text scoreText("Score", font);

// =====================================
// INICIALIZACIÓN DEL JUEGO
// =====================================
void Begin(const sf::Window& window)
{
    // Cargar automáticamente todas las texturas.
    for(auto&file : std::filesystem::directory_iterator("./resource/textures/"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".png"
                || file.path().extension() == ".jpeg"))
        {
            Resources::textures[file.path().filename().string()]
                .loadFromFile(file.path().string());
        }
    }

    // Cargar automáticamente todos los sonidos.
    for(auto&file : std::filesystem::directory_iterator("./resource/sounds/"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".ogg"
                || file.path().extension() == ".wav"))
        {
            Resources::sounds[file.path().filename().string()]
                .loadFromFile(file.path().string());
        }
    }

    
    // Configuración de música de fondo.
    music.openFromFile("./resource/sounds/music.ogg");
    music.setLoop(true);
    music.setVolume(35);

    // Configuración de textos UI.
    font.loadFromFile("./resource/fonts/SuperMarioBros.ttf");
    coinsText.setFillColor(sf::Color::White);
    coinsText.setOutlineColor(sf::Color::Black);
    coinsText.setOutlineThickness(1.0f);
    coinsText.setScale(0.1f,0.1f);

    livesText.setFillColor(sf::Color::White);
    livesText.setOutlineColor(sf::Color::Black);
    livesText.setOutlineThickness(1.0f);
    livesText.setScale(0.1f,0.1f);

    //Configuración texto score
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(1.0f);

    scoreText.setScale(0.1f,0.1f);

    // Comenzar siempre con score en cero.
    ScoreManager::ResetCurrentScore();
    // Inicializar Box2D.
    Physics::Init();

    // Construir mapa desde imagen.
    sf::Image image{};
    image.loadFromFile("./resource/textures/map (1).png");
    mario.position = map.CreateFromImage(image, objects);
    mario.spawnPosition = mario.position;

    // Inicializar jugador.
    mario.Begin();
    // Inicializar todos los objetos creados por el mapa.
    for(auto& object : objects)
    {
        object->Begin();
    }

    music.play();
    StartScoreThread();
    StartEnemyThread();
    StartCoinStatsThread();
    

    
}

// =====================================
// ACTUALIZACIÓN GLOBAL
// =====================================
void Update(float deltaTime)
{
    // El juego se pausa automáticamente
    // durante victoria o derrota.
    bool paused = mario.IsDead() || mario.HasWon();

    // física solo si no está pausado
    if(!paused)
    {
        Physics::Update(deltaTime);
    }

    // Mario SIEMPRE se actualiza
    mario.Update(deltaTime);

    camera.position = mario.position;
    NotifyEnemyThread();

    // enemigos/objetos solo si no está pausado
    if(!paused)
    {
        std::lock_guard<std::mutex> updateLock(objectsMutex);

        for(auto& object : objects)
        {
            object->Update(deltaTime);
        }
    }

    // =====================================
    // DESTRUCCIÓN SEGURA
    // =====================================
    //
    // Primero destruimos física.
    // Luego eliminamos memoria.
    //
    // Esto evita crashes de Box2D.
    // =====================================
    std::lock_guard<std::mutex> destroyLock(objectsMutex);
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
                if(Flag* flag = dynamic_cast<Flag*>(object))
                {
                    flag->DestroyPhysics();
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

// =====================================
// RENDER DEL MUNDO
// =====================================
void Render (Renderer& renderer)
{
    // Dibujar fondo.
    renderer.Draw (Resources::textures["background.png"], camera.position, camera.GetViewSize());
    map.Draw(renderer);
    mario.Draw(renderer);

    // Dibujar objetos dinámicos.
    std::lock_guard<std::mutex> lock(objectsMutex);

    for(auto& object : objects)
    {
        object->Render(renderer);
    }
    // Dibujar colisiones de depuración.
    Physics::DebugDraw(renderer);
}

// =====================================
// INTERFAZ DE USUARIO
// =====================================
void RenderUI(Renderer& renderer)
{
    // Mostrar cantidad de monedas.
    coinsText.setPosition(
        -camera.GetViewSize() / 2.0f + sf::Vector2f(2.0f, 1.0f)
    );

    coinsText.setString(
        "Coins: " + std::to_string(mario.GetCoins())
    );

    renderer.target.draw(coinsText);

    // Mostrar vidas restantes.
    livesText.setPosition(
        -camera.GetViewSize() / 2.0f + sf::Vector2f(2.0f, 3.0f)
    );

    livesText.setString(
        "Lives: " + std::to_string(mario.GetLives())
    );

    renderer.target.draw(livesText);

    // Mostrar score
    scoreText.setPosition(
        -camera.GetViewSize() / 2.0f +
        sf::Vector2f(2.0f, 5.5f)
    );

    scoreText.setString(
        "Score: " +
        std::to_string(
            ScoreManager::currentScore
        )
    );

    renderer.target.draw(scoreText);

    // =========================
    // GAME OVER
    // =========================

    if(mario.IsDead())
    {
        sf::Text gameOver("GAME OVER", font);

        gameOver.setFillColor(sf::Color::Red);
        gameOver.setOutlineColor(sf::Color::Black);
        gameOver.setOutlineThickness(2.0f);

        gameOver.setScale(0.14f, 0.14f);

        gameOver.setPosition(-6.0f, -2.0f);

        renderer.target.draw(gameOver);

        sf::Text restart("PRESS ENTER TO PLAY AGAIN", font);

        restart.setFillColor(sf::Color::White);
        restart.setOutlineColor(sf::Color::Black);
        restart.setOutlineThickness(1.0f);

        restart.setScale(0.05f, 0.05f);

        restart.setPosition(-7.0f, 1.5f);

        renderer.target.draw(restart);
    }

    // =========================
    // YOU WIN
    // =========================

    if(mario.HasWon())
    {
        sf::Text win("YOU WIN!", font);

        win.setFillColor(sf::Color::Yellow);
        win.setOutlineColor(sf::Color::Black);
        win.setOutlineThickness(2.0f);

        win.setScale(0.14f, 0.14f);

        win.setPosition(-5.0f, -2.0f);

        renderer.target.draw(win);

        sf::Text restart("PRESS ENTER TO RESTART", font);

        restart.setFillColor(sf::Color::White);
        restart.setOutlineColor(sf::Color::Black);
        restart.setOutlineThickness(1.0f);

        restart.setScale(0.05f, 0.05f);

        restart.setPosition(-7.0f, 1.5f);

        renderer.target.draw(restart);
    }
}

// Eliminación directa de objetos.
void DeleteObject(Object* object)
{
    std::lock_guard<std::mutex> lock(objectsMutex);

    const auto& it =
        std::find(objects.begin(), objects.end(), object);

    if(it != objects.end())
    {
        delete *it;
        objects.erase(it);
    }
}

// =====================================
// REINICIO COMPLETO DEL NIVEL
// =====================================
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
        if(Flag* flag = dynamic_cast<Flag*>(object))
        {
            flag->DestroyPhysics();
        }

        delete object;
    }

    // Vaciar lista global.
    objects.clear();

    // =========================
    // RECARGAR MAPA
    // =========================

    sf::Image image{};

    image.loadFromFile("./resource/textures/map (1).png");

    mario.position = map.CreateFromImage(image, objects);
    mario.spawnPosition = mario.position;

    // =========================
    // RESET MARIO
    // =========================
    // Reiniciar score para nueva partida.
    ScoreManager::ResetCurrentScore();
    mario.Reset();

    // =========================
    // RECREAR OBJETOS
    // =========================

    for(auto& object : objects)
    {
        object->Begin();
    }
}

void RenderScores(sf::RenderWindow& window)
{
    std::vector<int> scores =
        ScoreManager::LoadScores();

    sf::Text title("TOP SCORES", font);

    title.setCharacterSize(50);
    title.setPosition(350,50);

    window.draw(title);

    for(size_t i = 0;
        i < scores.size() && i < 10;
        i++)
    {
        sf::Text scoreText(
            std::to_string(i + 1)
            + ". "
            + std::to_string(scores[i]),
            font
        );

        scoreText.setCharacterSize(40);

        scoreText.setPosition(
            350,
            150 + i * 60
        );

        window.draw(scoreText);
    }
}

bool IsGameOver()
{
    return mario.IsDead();
}

bool HasPlayerWon()
{
    return mario.HasWon();
}