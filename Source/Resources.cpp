#include "Resources.h"

/// Definición del contenedor global de texturas.
std::unordered_map<std::string, sf::Texture>
    Resources::textures{};

/// Definición del contenedor global de sonidos.
std::unordered_map<std::string, sf::SoundBuffer>
    Resources::sounds{};