#include "head.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <random>

void mainMenu() {
    std::cout << "Главное меню" << std::endl;
    std::cout << "1. Игра против компьютера" << std::endl;
    std::cout << "2. Игра для двух игроков" << std::endl;
    std::cout << "3. Правила игры" << std::endl;
    std::cout << "4. Выход из игры" << std::endl;
    std::cout << std::endl;
    std::cout << "Ваш выбор:" << std::endl;
}

int isNumber() {
    int number;
    while (true) {
        std::cin >> number;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Ошибка ввода. Введите число:" << std::endl;
            std::cin.ignore(1000, '\n');
        } else {
            return number;
        }
    }
}

void gameRules() {
    std::cout << "\nПРАВИЛА ИГРЫ" << std::endl;
    std::cout << "1) Загадывается " << DIGITS_CNT << "-значное число"<< std::endl;
    std::cout << "2) Цифры в числе не повторяются" << std::endl;
    std::cout << "3) Цифры в числе от " << MIN_DIGIT << " до " << MAX_DIGIT << std::endl;
    std::cout << "4) Число МОЖЕТ начинаться с цифры 0" << std::endl;
    std::cout << "5) 'Быки' - правильные цифры на правильных местах" << std::endl;
    std::cout << "6) 'Коровы' - правильные цифры на неправильных местах" << std::endl;
    std::cout << "7) Какие именно цифры угаданы - не сообщается" << std::endl;
    std::cout << "Пример: загаданное число - 7890, попытка - 7609" << std::endl;
    std::cout << "Вывод: кол-во быков: 1, кол-во коров: 2\n" << std::endl;
}

std::string generateSecretNum() {
    std::random_device rd; // создание объекта random_device для получения аппаратно-случайных чисел
    std::mt19937 generator(rd()); // создание генератора Mersenne Twister (mt19937) и инициализация его
                                     // значением из random_device. Mersenne Twister - это алгоритм, генерирующий псевдослучайные числа.
    std::string digits = "0123456789";

    std::shuffle(digits.begin(), digits.end(), generator);

    return digits.substr(0, 4);
}

bool isValidInput(const std::string& player_number) {
    if (player_number.length() != DIGITS_CNT) {
        return false;
    }

    for (char c : player_number) {
        if (!isdigit(c)) {
            return false;
        }
    }

    for (int i = 0; i < DIGITS_CNT; i++) {
        for (int j = i + 1; j < DIGITS_CNT; j++) {
            if (player_number[i] == player_number[j]) {
                return false;
            }
        }
    }

    return true;
}

AttemptResult guessCheck(const std::string& secret, const std::string& guess) {
    AttemptResult result = {0, 0};

    for (int i = 0 ; i < DIGITS_CNT; i++) {
        if (guess[i] == secret[i]) {
            result.bulls++;
        }
    }

    for (int i = 0 ; i < DIGITS_CNT; i++) {
        for (int j = 0 ; j < DIGITS_CNT; j++) {
            if (guess[i] == secret[j] && i != j) {
                result.cows++;
                break;
            }
        }
    }

    return result;
}

void saveGameLogOnePlayer(const std::string& player_name, int attempts,
    const std::string& secret, bool won) {
    std::ofstream log_file("game_log.txt", std::ios::app);

    if (log_file.is_open()) {
        log_file << "Режим игры: Игра против компьютера." << std::endl;
        log_file << "Игрок: " << player_name << std::endl;
        log_file << "Загаданное число: " << secret << std::endl;
        log_file << "Кол-во попыток: " << attempts << std::endl;
        log_file << "Результат игры: " << (won ? "Победа" : "Поражение") << std::endl;
        for (int i=0; i<35; i++) {
            log_file << "-";
        }
        log_file << std::endl;
        log_file.close();
        std::cout << "\nЛог игры сохранен в файл game_log.txt\n" << std::endl;
    } else {
        std::cout << "\nНе получилось открыть файл game_log.txt\n" << std::endl;
    }
}

void saveGameLogTwoPlayers(const std::string& player1, const std::string& player2,
    const std::string& secret, int attempts, bool won) {
    std::ofstream log_file("game_log.txt", std::ios::app);

    if (log_file.is_open()) {
        log_file << "Режим игры: Два игрока." << std::endl;
        log_file << "Игроки: "<< player1 << " и " << player2 << std::endl;
        log_file << "Загадывал: " << player1 << std::endl;
        log_file << "Отгадывал: " << player2 << std::endl;
        log_file << "Загаданное число: " << secret << std::endl;
        log_file << "Кол-во попыток: " << attempts << std::endl;
        log_file << "Результат игры: ";
        if (won) {
            log_file << "Победа (" << player2 << " угадал число)" << std::endl;
        } else {
            log_file << "Поражение (" << player2 << " не угадал число)" << std::endl;
        }
        for (int i=0; i<35; i++) {
            log_file << "-";
        }
        log_file << std::endl;
        log_file.close();
        std::cout << "\nЛог игры сохранен в файл game_log.txt\n" << std::endl;
    } else {
        std::cout << "\nНе получилось открыть файл game_log.txt\n" << std::endl;
    }
}

std::string getAttemptsEnd(int attempts) {
    int mod10 = attempts % 10;
    int mod100 = attempts % 100;

    if (mod100 >= 11 && mod100 <= 14) return "попыток!";
    if (mod10 == 1) return "попытку!";
    if (mod10 >= 2 && mod10 <= 4) return "попытки!";
    return "попыток!";
}

void playVsComputer() {
    std::string secret = generateSecretNum();
    int attempts = 0;
    bool guessed = false;

    std::cout << "Режим игры: Игра против компьютера.\n" << std::endl;

    std::cout << "Компьютер загадал " << DIGITS_CNT << "-значное число." << std::endl;
    std::cout << "Цифры не повторяются. Попробуйте угадать!" << std::endl;
    std::cout << "Для выхода введите 'exit'" << std::endl;
    std::cout << "Для новой игры введите 'new'" << std::endl;

    std::cin.ignore(1000, '\n');
    std::cout << "\nВведите ваше имя/никнейм:" << std::endl;
    std::string player_name;
    getline(std::cin, player_name);

    while (!guessed) {
        for (int i=0; i<20; i++) {
            std::cout << "-";
        }
        std::cout << "\nВаше число: ";
        std::string guess;
        std::cin >> guess;

        if (guess == "exit") {
            std::cout << "Игра окончена. Загаданное число: " << secret << std::endl;
            saveGameLogOnePlayer(player_name, attempts, secret, false);
            return;
        } else if (guess == "new") {
            std::cout << "Начинаем новую игру." << std::endl;
            playVsComputer();
            return;
        } else if (!isValidInput(guess)) {
            std::cout << "Ошибка! Введите " << DIGITS_CNT
            << " уникальные цифры от 0 до 9" << std::endl;
            continue;
        }

        attempts++;
        AttemptResult result = guessCheck(secret, guess);

        std::cout << "Быки: " << result.bulls << ", Коровы: " << result.cows << std::endl;

        if (result.bulls == DIGITS_CNT) {
            std::string end = getAttemptsEnd(attempts);
            guessed = true;
            std::cout << "\nПоздравляем! Вы угадали число " << secret
                 << " за " << attempts << " " << end << std::endl;
            saveGameLogOnePlayer(player_name, attempts, secret, true);
        }
    }
}

void playTwoPlayers() {
    std::cout << "Режим игры: Два игрока.\n" << std::endl;

    std::string player1, player2;
    std::cout << "Введите имя первого игрока: " << std::endl;
    std::cin >> player1;
    std::cout << "Введите имя второго игрока: " << std::endl;
    std::cin >> player2;

    std::cout << player1 << ", загадывайте " << DIGITS_CNT
         << "-значное число:" << std::endl;

    std::string secret;
    while (true) {
        std::cin >> secret;

        if (secret == "exit") {
            return;
        }

        if (isValidInput(secret)) {
            break;
        }

        std::cout << "Неверное число! Введите " << DIGITS_CNT
             << " уникальные цифры: ";
    }

    for (int i = 0; i < 50; i++) {
        std::cout << std::endl;
    }

    std::cout << player2 << ", угадывайте число!" << std::endl;
    std::cout << "Для выхода введите 'exit'" << std::endl;
    std::cout << "Для новой игры введите 'new'" << std::endl;

    int attempts = 0;
    bool guessed = false;

    while (!guessed) {
        for (int i=0; i<20; i++) {
            std::cout << "-";
        }
        std::cout << "\n" << player2 << ", ваша попытка: ";
        std::string guess;
        std::cin >> guess;

        if (guess == "exit") {
            std::cout << "Игра окончена. Загаданное число: " << secret << std::endl;
            saveGameLogTwoPlayers(player1, player2, secret, attempts, false);
            return;
        }

        if (guess == "new") {
            playTwoPlayers();
            return;
        }

        if (!isValidInput(guess)) {
            std::cout << "Ошибка! Введите " << DIGITS_CNT
                 << " уникальные цифры" << std::endl;
            continue;
        }

        attempts++;
        AttemptResult result = guessCheck(secret, guess);

        std::cout << "Быки: " << result.bulls << ", Коровы: " << result.cows << std::endl;

        if (result.bulls == DIGITS_CNT) {
            guessed = true;
            std::string word = getAttemptsEnd(attempts);
            std::cout << "\n" << player2 << " угадал(-а) число " << secret
                 << " за " << attempts << " " << word << std::endl;
            saveGameLogTwoPlayers(player1, player2, secret, attempts, true);
        }
    }
}