// Timer.h
#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "./Color.h"

std::time_t stringToTimeT(const std::string &dateString);

class Task;

class Timer
{
private:
    int timerId;
    std::time_t dueTime;
    Task *task;

public:
    Timer(int id, std::time_t due, Task *t);
    void start();
    void stop();
    std::string getTimeRemaining() const;
    std::string getDueDate() const;
};

#endif
