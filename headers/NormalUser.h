#ifndef NORMALUSER_H
#define NORMALUSER_H
#include "User.h"
#include "Task.h"
#include "Group.h"
class NormalUser : public User
{
public:
    NormalUser(int id, std::string uname, std::string mail, std::string pass);
    std::vector<Task> viewGroupTasks(Group &group);
    void startTaskTimer(Task &task);
    void stopTaskTimer(Task &task);
};
#endif