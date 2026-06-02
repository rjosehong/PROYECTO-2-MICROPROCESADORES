#include "ScoreManager.h"

#include <fstream>
#include <algorithm>

/// =====================================
/// SCORE ACTUAL GLOBAL
/// =====================================
int ScoreManager::currentScore = 0;

/// =====================================
/// AGREGAR PUNTOS
/// =====================================
void ScoreManager::AddScore(int points)
{
    currentScore += points;
}

/// =====================================
/// REINICIAR SCORE
/// =====================================
void ScoreManager::ResetCurrentScore()
{
    currentScore = 0;
}

/// =====================================
/// GUARDAR SCORE EN DISCO
/// =====================================
/// Se guarda una línea por partida
void ScoreManager::SaveCurrentScore(int gameMode)
{
    std::ofstream file(
        "./resource/scores.txt",
        std::ios::app
    );

    if(file.is_open())
    {
        file << currentScore
             << " "
             << gameMode
             << "\n";
    }
}

/// =====================================
/// CARGAR SCORES
/// =====================================
std::vector<int> ScoreManager::LoadScores(int mode)
{
    std::vector<int> scores;

    std::ifstream file(
        "./resource/scores.txt"
    );

    int score;
    int storedMode;

    while (file >> score >> storedMode)
    {
        if (storedMode == mode)
        {
            scores.push_back(score);
        }
    }

    std::sort(
        scores.begin(),
        scores.end(),
        std::greater<int>()
    );

    return scores;
}