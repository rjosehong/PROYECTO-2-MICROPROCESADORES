
// =====================================
// GAME.H
// =====================================
//
// Declaraciones principales del juego.
//
// Este archivo expone:
//
// - Inicio del juego
// - Actualización lógica
// - Renderizado
// - UI
// - Reinicio de nivel
// - Estado de victoria y derrota
//
// =====================================#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Renderer.h"
#include "Camera.h"
#include "Object.h"

/// Cámara principal utilizada por todo el juego.
extern Camera camera;

/// Inicializa recursos, mapa, jugador y objetos.
///
/// @param window ventana principal.
void Begin(const sf::Window& window);

/// Actualiza toda la lógica del juego.
///
/// @param deltaTime tiempo transcurrido
/// desde el frame anterior.
void Update(float deltaTime);
/// Renderiza el mundo del juego.
void Render(Renderer& renderer);
//Renderiza la interfaz de usuario
void RenderUI(Renderer& renderer);

/// Elimina un objeto del juego.
void DeleteObject(Object* object);

/// Muestra la pantalla de puntuaciones.
void RenderScores(sf::RenderWindow& window);

/// Reinicia completamente el nivel actual.
void RestartGame(const sf::Window& window);
/// Indica si el jugador perdió.
bool IsGameOver();
/// Indica si el jugador ganó el nivel.
bool HasPlayerWon();
/// Configura el modo de juego (1 o 2 jugadores).
void SetPlayerMode(int players);
int GetCurrentPlayer();
bool IsTwoPlayerMode();