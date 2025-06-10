#pragma once
#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>

class HighScoreManager {
public:
    static const int VERY_LARGE_NUMBER = 999999;

    static int loadUserHighScore(const std::string& username);
    static void saveUserHighScore(const std::string& username, int moves);
    static void showFinalStats(const std::string& username, int moves);
};

#endif
