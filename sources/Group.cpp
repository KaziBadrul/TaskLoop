#include "../headers/Group.h"
#include <iostream>
Group::Group(int id, std::string name) : groupId(id), groupName(name) {}

Group::~Group()
{
    for (auto member : members)
    {
        delete member;
    }
}

int Group::getGroupId() const
{
    return groupId;
}

std::string Group::getGroupName() const
{
    return groupName;
}

void Group::addUser(User *user)
{
    members.push_back(user);
}

void Group::removeUser(User *user)
{
    // members.erase(std::remove(members.begin(), members.end(), user), members.end());
}

std::vector<User *> Group::viewGroupMembers()
{
    return members;
}
std::vector<Task> Group::viewGroupTasks()
{
    return {};
}
