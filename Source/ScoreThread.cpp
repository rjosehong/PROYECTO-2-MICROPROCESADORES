// =====================================
// SCORE THREAD
// =====================================
//
// Sistema concurrente encargado de:
//
// - Recibir eventos de puntuación.
// - Procesarlos en segundo plano.
// - Actualizar el score global.
//
// Utiliza:
//
// - std::thread
// - std::mutex
// - std::condition_variable
//
// =====================================
#include "ScoreThread.h"
#include "ScoreManager.h"

#include <thread>

std::queue<int> scoreQueue;

std::mutex scoreMutex;

std::condition_variable scoreCV;

bool running = true;

std::thread scoreThread;

/// Hilo principal del sistema de score.
///
/// Permanece bloqueado hasta recibir
/// nuevos eventos de puntuación.
///
/// Cuando un objeto del juego genera
/// puntos (moneda, enemigo, bandera),
/// se inserta un evento en scoreQueue
/// y se despierta este hilo mediante
/// scoreCV.notify_one().
void ScoreWorker()
{
    while(running)
    {
        std::unique_lock<std::mutex> lock(scoreMutex);

        scoreCV.wait(lock, []()
        {
            return !scoreQueue.empty() || !running;
        });

        if(!running)
        {
            break;
        }

        while(!scoreQueue.empty())
        {
            int points = scoreQueue.front();

            scoreQueue.pop();

            ScoreManager::AddScore(points);
        }
    }
}

/// Crea e inicia el hilo encargado
/// de procesar eventos de puntuación.
void StartScoreThread()
{
    scoreThread = std::thread(ScoreWorker);
}
/// Solicita la finalización del hilo
/// ScoreWorker y espera a que termine
/// correctamente antes de cerrar el juego.

void StopScoreThread()
{
    running = false;

    scoreCV.notify_all();

    if(scoreThread.joinable())
    {
        scoreThread.join();
    }
}