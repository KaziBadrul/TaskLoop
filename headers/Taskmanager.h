#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "./Database.h"
#include "./User.h"
#include "./Group.h"
#include "./Task.h"
#include "./Color.h"
#include "./Admin.h"
#include <ctime>
#include <iomanip>
#include <fstream>

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
    void handleSelectedGroupMenu(int groupId, Group &group);
    void handleMemberMenu(Group &group, int groupId);
    void addMember(Admin &admin, Group &group, int groupId);
    void removeMember(Admin &admin, Group &group, int groupId);
    void handleViewTasks(Group group, int groupId);
    void handleTaskMenu(Group group, int groupId);
    void handleAdminMenu(Admin &admin, Group &group, std::vector<User> members);
    // void handleUserMenu(Group &group, int groupId);
    void createGroup();
    void viewGroups();
    void createTask(int groupId);
    void completeTask(int groupId);
    void saveTaskToFile(int groupId);
};

#endif