#pragma once

/// =====================================
/// ENEMY THREAD
/// =====================================
///
/// Hilo encargado de procesar la lógica
/// automática de los enemigos.
///
/// =====================================

/// Inicia el hilo de enemigos.
void StartEnemyThread();

/// Detiene el hilo de enemigos.
void StopEnemyThread();

/// Despierta al hilo de enemigos.
void NotifyEnemyThread();
