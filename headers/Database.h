#ifndef DATABASE_H
#define DATABASE_H

#include "./sqlite3.h"
#include "User.h"
#include "Group.h"
#include "Task.h"
#include <string>
#include <iostream>

class Database
{
private:
    sqlite3 *db;

public:
    Database(const std::string &dbName);
    ~Database();

    // FUNCTIONS FOR USER AUTHENTICATION
    bool createUser(const std::string &username, const std::string &email, const std::string &password);
    bool loginUser(const std::string &username, const std::string &password, User &user);

    // FUNCTIONS FOR GROUPS AND TASKS
    bool createGroup(const std::string &groupName, int ownerId);
    bool addUserToGroup(int groupId, int userId);
    bool removeUserFromGroup(int groupId, int userId);
    bool createTask(const std::string &taskName, const std::string &description, const std::string &dueDate, int priority, int groupId, int userId);
    std::vector<Group> getUserGroups(int userId);
    std::vector<User> getGroupMembers(int groupId);
    bool addGroupMember(int groupId, const std::string &username);
    bool removeGroupMember(int groupId, const std::string &username);
    std::vector<Task> getGroupTasks(int groupId);
    bool updateTaskStatus(int taskId, int newStatus);
    bool completeTask(int taskId);
};

#endif
