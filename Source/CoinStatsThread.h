#pragma once

/// =====================================
/// COIN STATS THREAD
/// =====================================
///
/// Hilo encargado de registrar eventos
/// relacionados con monedas.
///
/// Utiliza semáforos POSIX para recibir
/// notificaciones desde el juego.
///
/// =====================================

/// Inicia el hilo.
void StartCoinStatsThread();

/// Detiene el hilo.
void StopCoinStatsThread();
/// Despierta el hilo indicando que una
/// moneda fue recolectada.
void NotifyCoinCollected();