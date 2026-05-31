#pragma once
#include <box2d/b2_body.h>
#include "Renderer.h"

class MyDebugDraw;
class Mario;
class Object;

/// Interfaz base para objetos que desean
/// recibir eventos de colisión de Box2D.
///
/// Mario implementa esta interfaz para
/// reaccionar cuando toca monedas,
/// enemigos, suelo o la bandera.
class ContactListener
{
public:
/// Se ejecuta cuando comienza una colisión.
///
/// @param self fixture perteneciente al objeto.
/// @param other fixture con el que colisionó.
    virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;

/// Se ejecuta cuando termina una colisión.
///
/// @param self fixture perteneciente al objeto.
/// @param other fixture con el que deja de colisionar.
    virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;
};

/// Tipos de objetos físicos utilizados
/// para identificar colisiones dentro
/// del mundo Box2D.
enum class FixtureDataType
{
    /// Fixture perteneciente a Mario.
    Mario,
    /// Fixture perteneciente a un tile del mapa.
    MapTile,
    /// Fixture perteneciente a un objeto dinámico.
    Object
};

/// Información personalizada asociada
/// a cada fixture de Box2D.
///
/// Permite identificar rápidamente
/// qué tipo de objeto participó
/// en una colisión.
struct FixtureData
{
    /// Tipo de fixture.
    FixtureDataType type;
    /// Objeto que recibirá eventos de colisión.
    ContactListener* listener;

    /// Información específica según el tipo.
    union 
    {
        /// Referencia al jugador.
        Mario* mario;
        /// Referencia a un objeto dinámico.
        Object* object;
        /// Coordenadas del tile dentro del mapa.
        struct { int mapX, mapY;};
    };
    
};

/// Clase estática encargada de administrar
/// el mundo físico Box2D.
class Physics
{
public:
/// Inicializa el sistema de físicas.
    static void Init();

/// Actualiza la simulación física.
///
/// @param deltaTime tiempo transcurrido
/// desde el frame anterior.
    static void Update(float deltaTime);

/// Dibuja las colisiones utilizando
/// el sistema DebugDraw de Box2D.
    static void DebugDraw(Renderer& renderer);

    /// Mundo físico global.
    static b2World world;
    /// Renderizador de depuración.
    static MyDebugDraw* debugDraw;
}; 