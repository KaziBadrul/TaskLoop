#ifndef ADMIN_H
#define ADMIN_H
#include "User.h"
#include "Group.h"
#include "Task.h"
class Admin : public User
{
private:
    int adminLevel;

public:
    Admin(int id, std::string uname, std::string mail, std::string pass, int level);
    void createGroup(Group &group);
    void assignTaskToGroup(Task &task, Group &group);
    std::vector<Group> viewAllGroups();
    void setTaskPriority(Task &task, PriorityLevel priority);
};
#endif