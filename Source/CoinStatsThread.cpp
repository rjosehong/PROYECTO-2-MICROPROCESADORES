#include "CoinStatsThread.h"

#include <thread>
#include <iostream>
#include <semaphore.h>

/// =====================================
/// VARIABLES GLOBALES
/// =====================================

sem_t coinSemaphore;

bool coinThreadRunning = true;

std::thread coinThread;

/// =====================================
/// HILO PRINCIPAL
/// =====================================
///
/// Permanece bloqueado esperando eventos
/// de monedas.
///
/// Cada vez que Mario recoge una moneda,
/// se realiza un sem_post() y este hilo
/// despierta.
///
/// =====================================

void CoinWorker()
{
    while(coinThreadRunning)
    {
        sem_wait(&coinSemaphore);

        if(!coinThreadRunning)
        {
            break;
        }

        std::cout
            << "[CoinThread] moneda recolectada"
            << std::endl;
    }
}

/// Inicia el hilo de estadísticas.
void StartCoinStatsThread()
{
    sem_init(
        &coinSemaphore,
        0,
        0
    );

    coinThread =
        std::thread(CoinWorker);
}

/// Detiene el hilo de estadísticas.
void StopCoinStatsThread()
{
    coinThreadRunning = false;

    sem_post(&coinSemaphore);

    if(coinThread.joinable())
    {
        coinThread.join();
    }

    sem_destroy(&coinSemaphore);
}

/// Notifica que una moneda fue
/// recolectada.
void NotifyCoinCollected()
{
    sem_post(&coinSemaphore);
}