#pragma once

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/// Contenedor global de recursos.
class Resources
{
public:

    /// Todas las texturas cargadas.
    static std::unordered_map<std::string, sf::Texture> textures;

    /// Todos los sonidos cargados.
    static std::unordered_map<std::string, sf::SoundBuffer> sounds;
};