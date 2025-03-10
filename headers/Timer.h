// Timer.h
#ifndef TIMER_H
#define TIMER_H

#include <string>

class Task;

class Timer
{
private:
    int timerId;
    std::string startTime;
    std::string endTime;
    Task *task;

public:
    Timer(int id, std::string start, std::string end, Task *t);
    void start();
    void stop();
};

#endif
