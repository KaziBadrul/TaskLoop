#include "../headers/Timer.h"
#include <iostream>

Timer::Timer(int id, std::time_t due, Task *t)
    : timerId(id), dueTime(due), task(t) {}

std::string Timer::getDueDate() const
{
    std::tm *dueDateTm = std::localtime(&dueTime);

    std::ostringstream oss;
    oss << std::put_time(dueDateTm, "%Y-%m-%d");

    return oss.str();
}

std::string Timer::getTimeRemaining() const
{
    std::time_t now = std::time(nullptr);
    double secondsLeft = difftime(dueTime, now);

    if (secondsLeft <= 0)
    {
        SetColor(91);
        return "Task overdue!";
    }

    int days = secondsLeft / 86400;
    int hours = (int)(secondsLeft / 3600) % 24;
    int minutes = (int)(secondsLeft / 60) % 60;
    int seconds = (int)secondsLeft % 60;

    std::ostringstream oss;
    SetColor(32);
    oss << days << "d " << hours << "h " << minutes << "m " << seconds << "s left";
    return oss.str();
}
void Timer::start()
{
    std::cout << "Timer started for task.\n";
}
void Timer::stop()
{
    std::cout << "Timer stopped for task.\n";
}