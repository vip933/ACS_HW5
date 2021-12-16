#include <iostream>
#include <thread>
#include <cstring>
#include <vector>
#include "Caller.h"
#include "fstream"

int main(int argc, char *argv[]) {
    // Количество болтунов.
    int number;

    if (argc == 3) {
        if (!strcmp(argv[1], "-n")) {
            try {
                number = std::stoi(argv[2]);
            } catch (std::invalid_argument ex) {
                std::cout << ex.what() << "\n";
                return 1;
            }
        } else if (!strcmp(argv[1], "-f")) {
            std::ifstream ifstream;
            ifstream.open(argv[2]);
            std::string str;
            getline(ifstream, str);
            try {
                number = std::stoi(str);
            } catch (std::invalid_argument ex) {
                std::cout << ex.what() << "\n";
                return 1;
            }
        } else {
            std::cout << "Неправильный аргумент (-f/-n).\n";
            std::cout << "Ожидалось: ./task -f file.txt or ./task -n 10\n";
            return 2;
        }
    } else {
        std::cout << "Неправильное количество аргументов.\n";
        std::cout << "Ожидалось: ./task -f file.txt or ./task -n 10\n";
        return 1;
    }
    // Создание болтунов.
    std::vector<Caller *> users(number);
    for (int i = 0; i < number; ++i) {
        users[i] = new Caller("Болтун с индексом " + std::to_string(i));
    }
    for (int i = 0; i < number; ++i) {
        users[i]->setContacts(users, number);
    }
    std::vector<std::thread> threads(number);

    std::cout << "Начало программы (Программа будет работать 10 секунд).\n";
    // Создание и запуск потоков.
    for (int i = 0; i < number; ++i) {
        threads[i] = std::thread(&Caller::run, users[i]);
    }

    // Рабочее время программы.
    std::this_thread::sleep_for(std::chrono::milliseconds(10'000));

    // Завершение работы.
    for (int i = 0; i < number; ++i) {
        users[i]->kill();
        threads[i].join();
    }

    std::cout << "Конец программы.\n";
}
