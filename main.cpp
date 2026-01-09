#include "head.h"
#include <iostream>

int main() {
    int choice;
    std::cout << "Игра 'Быки и коровы'" << std::endl;

    do {
        mainMenu();
        choice = isNumber();

        switch (choice) {
            case 1: playVsComputer(); break;
            case 2: playTwoPlayers(); break;
            case 3: gameRules(); break;
            case 4: std::cout << "Выход из программы. До свидания!"; break;
            default: std::cout << "Некорректный ввод. Введите число от 1 до 4.\n\n"; break;
        }
    } while (choice != 4);

    return 0;
}