#include "../headers/Admin.h"

#include <iostream>

Admin::Admin(int id, std::string uname, std::string mail, std::string pass)
    : User(id, uname, mail, pass) {}

int Admin::getId() const
{
    return userId;
}

void Admin::addMember(Database &db, Group &group)
{
    int groupId = group.getGroupId();
    std::cout << "Enter member username: ";
    std::string username;
    std::getline(std::cin, username);
    if (db.addGroupMember(groupId, username))
    {
        group.addUser(new User(username));
        std::cout << "Member added successfully!\n";
    }
    else
    {
        std::cout << "Failed to add member.\n";
    }
}

void Admin::removeMember(Database &db, Group &group)
{
    int groupId = group.getGroupId();
    std::cout << "Enter member username: ";
    std::string username;
    std::getline(std::cin, username);
    if (db.removeGroupMember(groupId, username))
    {
        std::cout << "Member removed successfully!\n";
    }
    else
    {
        std::cout << "Failed to remove member.\n";
    }
}
