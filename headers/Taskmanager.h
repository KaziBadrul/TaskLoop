#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "Database.h"
#include "User.h"
#include "Group.h"
#include "Task.h"

class TaskManager
{
private:
    Database db;
    User *currentUser;

public:
    TaskManager(const std::string &dbName);
    ~TaskManager();

    void start();
    void showMainMenu();
    void handleSignUp();
    void handleLogin();
    void handleGroupMenu();
    void handleTaskMenu(int groupId);
    void createGroup();
    void viewGroups();
    void createTask(int groupId);
    void completeTask(int groupId);
};

#endif