#include "Animation.h"

/// Constructor.
/// Inicializa duración y frames.
Animation::Animation(float lenght,std::vector<AnimFrame> frames)
    : lenght(lenght), frames(frames)
{
}

/// Incrementa el tiempo interno de la animación.
void Animation::Update(float deltaTime)
{
    time += deltaTime;
}

/// Devuelve la textura correspondiente al tiempo actual.
sf::Texture Animation::GetTexture()
{
    // Reinicia el ciclo de animación cuando supera la duración.
    while (time > lenght)
    {
        time -= lenght;
    }

    // Busca el frame correspondiente.
    for (const auto& frame : frames)
    {
        if (time >= frame.time)
        {
            return frame.texture;
        }
    }

    // Retorna textura vacía si no encuentra frame.
    return sf::Texture();
}