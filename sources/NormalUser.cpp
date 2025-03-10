#include "../headers/NormalUser.h"
#include <iostream>
NormalUser::NormalUser(int id, std::string uname, std::string mail, std::string pass)
    : User(id, uname, mail, pass) {}
std::vector<Task> NormalUser::viewGroupTasks(Group &group)
{
    return {};
}
void NormalUser::startTaskTimer(Task &task)
{
    std::cout << "Started timer for task.\n";
}
void NormalUser::stopTaskTimer(Task &task)
{
    std::cout << "Stopped timer for task.\n";
}