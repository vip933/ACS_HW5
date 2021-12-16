//
// Created by Maksim on 15.12.2021.
//

#ifndef MDLEYPUNSKIY_ACS_HW5_CALLER_H
#define MDLEYPUNSKIY_ACS_HW5_CALLER_H

#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <vector>
#include <semaphore.h>
#include <condition_variable>
#include <utility>

class Caller {
private:
    std::condition_variable cv;
    std::mutex preCall;
    std::string name_;
    bool isAlive;
    bool isTalking_;
    std::vector<Caller *> contacts_;
    int contacts_len_;
    std::mutex callMutex;
    int milliseconds;

public:
    explicit Caller(std::string);

    void setContacts(std::vector<Caller*>, int);

    void callHere(Caller*);

    static inline int random(int, int);

    void run();

    void kill();
};


#endif //MDLEYPUNSKIY_ACS_HW5_CALLER_H
