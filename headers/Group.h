#ifndef GROUP_H
#define GROUP_H
#include <string>
#include <vector>
#include "User.h"
#include "Task.h"
class Group
{
private:
    int groupId;
    std::string groupName;
    std::vector<User *> members;
    int adminId;
    int ownerId;

public:
    Group(int id, std::string name, int ownerId);
    ~Group();
    int getGroupId() const;
    std::string getGroupName() const;
    int getOwnerId() const;
    int getAdminId() const;
    void addUser(User *user);
    void removeUser(User *user);
    std::vector<User *> viewGroupMembers();
    std::vector<Task> viewGroupTasks();
};
#endif