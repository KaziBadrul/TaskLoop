#include "../headers/Timer.h"
#include <iostream>
Timer::Timer(int id, std::string start, std::string end, Task *t)
    : timerId(id), startTime(start), endTime(end), task(t) {}
void Timer::start()
{
    std::cout << "Timer started for task.\n";
}
void Timer::stop()
{
    std::cout << "Timer stopped for task.\n";
}