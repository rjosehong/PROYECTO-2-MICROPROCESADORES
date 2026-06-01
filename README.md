# PROYECTO-2-MICROPROCESADORES
# Mario Bros Concurrente

## Universidad del Valle de Guatemala

**Facultad de Ingeniería**

**Curso:** CC3086 Programación de Microprocesadores

**Proyecto:** Mario Bros Concurrente utilizando C++, SFML, Box2D y Programación Multihilo

---

# Descripción General

Este proyecto consiste en una adaptación del juego clásico Mario Bros desarrollada en C++. El jugador controla a Mario a través de un nivel de plataformas donde debe recolectar monedas, derrotar enemigos y llegar a la bandera final para completar el nivel.

La implementación utiliza SFML para la interfaz gráfica y Box2D para la simulación física y detección de colisiones. Además, incorpora técnicas de programación concurrente mediante múltiples hilos de ejecución y mecanismos de sincronización.

---

# Características

* Menú principal interactivo.
* Pantalla de instrucciones.
* Sistema de puntajes.
* Persistencia de scores en archivo.
* Sistema de vidas.
* Enemigos con movimiento automático.
* Recolección de monedas.
* Meta final mediante bandera.
* Reinicio de partida.
* Física y colisiones mediante Box2D.
* Programación concurrente mediante hilos.

---

# Tecnologías Utilizadas

## Lenguaje

* C++17

## Librerías

* SFML Graphics
* SFML Window
* SFML Audio
* SFML System
* Box2D
* STL Threads
* STL Mutex
* STL Condition Variables
* POSIX Semaphores

---

# Compilación

Compilar utilizando:

```bash
g++ Source/*.cpp -o mario_bros -std=c++17 -pthread -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lbox2d
```

---

# Ejecución

```bash
./mario_bros
```

---

# Controles

| Tecla  | Acción                                    |
| ------ | ----------------------------------------- |
| A      | Mover izquierda                           |
| D      | Mover derecha                             |
| SPACE  | Saltar                                    |
| ENTER  | Reiniciar partida tras victoria o derrota |
| ESC    | Volver al menú principal                  |

---

# Arquitectura Concurrente

El proyecto utiliza múltiples hilos para distribuir responsabilidades específicas dentro del juego.

## Hilo Principal

Responsable de:

* Renderizado
* Actualización general
* Entrada del usuario
* Gestión de estados del juego

---

## ScoreWorker

Archivo:

* ScoreThread.cpp

Responsabilidad:

* Procesar eventos de puntuación generados por monedas, enemigos y victoria.

Sincronización utilizada:

* std::mutex
* std::condition_variable

Recursos compartidos:

* scoreQueue
* currentScore

---

## EnemyWorker

Archivo:

* EnemyThread.cpp

Responsabilidad:

* Procesar la lógica automática de enemigos.

Sincronización utilizada:

* std::mutex
* std::condition_variable

Recursos compartidos:

* objects
* objectsMutex

---

## CoinWorker

Archivo:

* CoinStatsThread.cpp

Responsabilidad:

* Procesar eventos relacionados con monedas recolectadas.

Sincronización utilizada:

* POSIX Semaphores

Recursos compartidos:

* coinSemaphore

---

# Mecanismos de Sincronización

## Mutex

Utilizados para proteger recursos compartidos y evitar condiciones de carrera.

### Mutex implementados

* gameMutex
* objectsMutex
* scoreMutex
* enemyMutex

---

## Variables de Condición

Permiten mantener hilos bloqueados hasta que exista trabajo pendiente.

### Variables implementadas

* scoreCV
* enemyCV

---

## Semáforos

Utilizados para la comunicación entre eventos del juego y el hilo de estadísticas de monedas.

### Semáforos implementados

* updateSemaphore
* coinSemaphore

---

# Sistema de Puntajes

| Evento            | Puntos |
| ----------------- | ------ |
| Moneda            | 100    |
| Enemigo derrotado | 200    |
| Completar nivel   | 1000   |

Los puntajes son almacenados en:

```text
resource/scores.txt
```

y posteriormente mostrados en la sección Top Scores.

---

# Estructura General

```text
Source/
│
├── Main.cpp
├── Game.cpp
├── Mario.cpp
├── Enemy.cpp
├── Coin.cpp
├── Map.cpp
├── Physics.cpp
│
├── ScoreManager.cpp
├── ScoreThread.cpp
├── EnemyThread.cpp
├── CoinStatsThread.cpp
│
├── Menu.cpp
├── Instructions.cpp
│
└── ...
```

---

# Autores

Proyecto desarrollado para el curso CC3086 Programación de Microprocesadores.
Alejandro Rustrian 25512
Rodrigo Navas 25589
Marco Antonio Soloj 25701 

Universidad del Valle de Guatemala.
