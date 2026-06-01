#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

/// Cola compartida utilizada para almacenar
/// eventos de puntuación generados por el juego.
///
/// Cada evento representa una cantidad de puntos
/// pendiente de ser procesada por el hilo ScoreWorker.
extern std::queue<int> scoreQueue;

/// Mutex encargado de proteger el acceso
/// concurrente a la cola de puntuación.
extern std::mutex scoreMutex;

/// Variable de condición utilizada para
/// despertar al hilo ScoreWorker cuando
/// existen nuevos eventos pendientes.
extern std::condition_variable scoreCV;

/// Inicializa y lanza el hilo encargado
/// de procesar eventos de puntuación.
void StartScoreThread();

/// Detiene de forma segura el hilo
/// encargado de procesar puntuaciones.
void StopScoreThread();