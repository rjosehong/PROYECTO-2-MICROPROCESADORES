#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "Object.h"

/// Clase encargada de construir y dibujar el mapa.
class Map
{
public:

    /// Constructor.
    /// @param cellSize tamaño lógico de cada celda.
    Map(float cellSize = 32.0f);

    /// Genera un tablero de prueba.
    void CreateCheckerboard(size_t width, size_t height);

    /// Construye el mapa a partir de una imagen.
    /// Cada color representa un objeto diferente.
    ///
    /// @param image imagen fuente.
    /// @param objects lista donde se insertan objetos dinámicos.
    ///
    /// @return posición inicial de Mario.
    sf::Vector2f CreateFromImage(
        const sf::Image& image,
        std::vector<Object*>& objects
    );

    /// Dibuja el mapa completo.
    void Draw(Renderer& renderer);

    /// Grid de texturas.
    ///
    /// Cada posición contiene:
    /// - nullptr si no hay tile.
    /// - puntero a textura si existe tile.
    std::vector<std::vector<sf::Texture*>> grid;

    /// Tamaño lógico de cada celda.
    float cellSize;

private:
};