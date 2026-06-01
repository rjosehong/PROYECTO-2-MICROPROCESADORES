#include "EnemyThread.h"
#include "Enemy.h"
#include "Object.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

/// =====================================
/// VARIABLES COMPARTIDAS
/// =====================================

bool enemyRunning = true;

std::thread enemyThread;

std::mutex enemyMutex;

std::condition_variable enemyCV;

/// Indica si existe trabajo pendiente.
bool enemyWorkPending = false;

extern std::vector<Object*> objects;

extern std::mutex objectsMutex;

/// =====================================
/// HILO PRINCIPAL DE ENEMIGOS
/// =====================================
///
/// Permanece dormido hasta recibir una
/// señal desde el hilo principal.
///
/// =====================================

void EnemyWorker()
{
    while(enemyRunning)
    {
        std::unique_lock<std::mutex> lock(enemyMutex);

        enemyCV.wait(lock, []()
        {
            return enemyWorkPending || !enemyRunning;
        });

        if(!enemyRunning)
        {
            break;
        }

        enemyWorkPending = false;

        lock.unlock();

        /// =====================================
        /// PROCESAR ENEMIGOS
        /// =====================================

        {
            std::lock_guard<std::mutex>
                objectsLock(objectsMutex);

            for(auto object : objects)
            {
                Enemy* enemy =
                    dynamic_cast<Enemy*>(object);

                if(enemy)
                {
                    enemy->ThreadUpdate();
                }
            }
        }
    }
}

/// Inicia el hilo encargado de procesar
/// la lógica de enemigos.
void StartEnemyThread()
{
    enemyThread =
        std::thread(EnemyWorker);
}

/// Detiene de forma segura el hilo
/// de enemigos.
void StopEnemyThread()
{
    enemyRunning = false;

    enemyCV.notify_all();

    if(enemyThread.joinable())
    {
        enemyThread.join();
    }
}

/// Notifica al hilo de enemigos que
/// existe trabajo pendiente.
void NotifyEnemyThread()
{
    {
        std::lock_guard<std::mutex>
            lock(enemyMutex);

        enemyWorkPending = true;
    }

    enemyCV.notify_one();
}