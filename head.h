#ifndef CPP_HEAD_H
#define CPP_HEAD_H
#include <string>

const int DIGITS_CNT = 4;
const int MIN_DIGIT = 0;
const int MAX_DIGIT = 9;

struct AttemptResult {
    int bulls;
    int cows;
};

void mainMenu();

int isNumber();
void gameRules();
std::string generateSecretNum();
bool isValidInput(const std::string& player_number);
AttemptResult guessCheck(const std::string& secret, const std::string& guess);
std::string getAttemptsEnd(int attempts);

void saveGameLogOnePlayer(const std::string& player_name, int attempts,
    const std::string& secret, bool won);
void saveGameLogTwoPlayers(const std::string& player1, const std::string& player2,
    const std::string& secret, int attempts, bool won);

void playVsComputer();
void playTwoPlayers();

#endif