#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>

class Resources
{
public:
    static std::unordered_map<std::string, sf::Texture> textures;

};