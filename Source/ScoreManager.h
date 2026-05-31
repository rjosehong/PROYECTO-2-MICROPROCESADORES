#pragma once

#include <vector>

/// =====================================
/// SCORE MANAGER
/// =====================================
/// Clase estática encargada de gestionar:
/// - Score actual de la partida.
/// - Guardado permanente de scores.
/// - Carga de puntuaciones históricas.
/// =====================================
class ScoreManager
{
public:

    /// Score actual de la partida.
    static int currentScore;

    /// Agrega puntos al score actual.
    ///
    /// @param points cantidad de puntos.
    static void AddScore(int points);

    /// Reinicia el score para una nueva partida.
    static void ResetCurrentScore();

    /// Guarda el score actual en disco.
    static void SaveCurrentScore();

    /// Carga todos los scores guardados.
    static std::vector<int> LoadScores();
};