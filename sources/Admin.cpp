#include "../headers/Admin.h"
#include <iostream>
Admin::Admin(int id, std::string uname, std::string mail, std::string pass, int level)
    : User(id, uname, mail, pass), adminLevel(level) {}
void Admin::createGroup(Group &group)
{
    std::cout << "Admin created group: " << group.viewGroupMembers().size() << " members.\n";
}
void Admin::assignTaskToGroup(Task &task, Group &group)
{
    std::cout << "Task assigned to group.\n";
}
std::vector<Group> Admin::viewAllGroups()
{
    return {};
}
void Admin::setTaskPriority(Task &task, PriorityLevel priority)
{
    std::cout << "Priority updated.\n";
}