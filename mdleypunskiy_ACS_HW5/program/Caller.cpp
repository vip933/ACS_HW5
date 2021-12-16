//
// Created by Maksim on 15.12.2021.
//

#include "Caller.h"


Caller::Caller(std::string name) {
    isAlive = true;
    name_ = name;
    isTalking_ = false;
    milliseconds = 0;
}

// Выбор действия болтуна.
void Caller::run() {
    while (isAlive) {
        // Попытка соедениться.
        if (!isTalking_) {
            int ind = random(0, contacts_len_ - 1);
            if (this != contacts_[ind]) {
                contacts_[ind]->callHere(this);
                cv.notify_all();
            }
            // Сон до конца времени.
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }
    }
}

void Caller::setContacts(std::vector<Caller *> contacts, int len) {
    contacts_ = std::move(contacts);
    contacts_len_ = len;
}

// Обработка звонка от caller.
void Caller::callHere(Caller *caller) {
    std::unique_lock<std::mutex> lck(preCall);
    // Проверка, вызывающий не занят.
    if (caller->isTalking_)
        return;
    // Проверка, что вызываемый не занят.
    if (isTalking_) {
        callMutex.lock();
        std::cout << "<" + caller->name_ + "> пытался позвонить <" + name_ + ">, но <" + name_ + "> занят\n";
        callMutex.unlock();
        return;
    }
    // Идет звонок.
    callMutex.lock();
    isTalking_ = true;
    caller->isTalking_ = true;
    std::cout << "Начали разговор: <" + caller->name_ + "> и <" + name_ + ">\n";
    // Рандомная продолжительность звонка.
    milliseconds = random(100, 300);
    caller->milliseconds = milliseconds;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "Закончили разговор: <" + caller->name_ + "> и <" + name_ + ">\n";
    isTalking_ = false;
    caller->isTalking_ = false;
    // Звонок закончен.
    callMutex.unlock();
}

// Завершение потока.
void Caller::kill() {
    isAlive = false;
}

// Генерация рандомного числа;
int Caller::random(int left, int right) {
    return left + rand() % (right - left + 1);
}
