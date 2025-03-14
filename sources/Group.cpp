#include "../headers/Group.h"
#include <iostream>
Group::Group(int id, std::string name, int ownerId) : groupId(id), groupName(name), ownerId(ownerId) {}

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

int Group::getOwnerId() const
{
    return ownerId;
}

int Group::getAdminId() const
{
    return adminId;
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
