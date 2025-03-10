#include "TaskManager.h"
#include <iostream>

TaskManager::TaskManager(const std::string &dbName) : db(dbName), currentUser(nullptr) {}

TaskManager::~TaskManager()
{
    delete currentUser;
}

void TaskManager::start()
{
    showMainMenu();
}

void TaskManager::showMainMenu()
{
    int choice;
    std::cout << "1. Sign Up\n2. Log In\n0. Exit\n";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice)
    {
    case 1:
        handleSignUp();
        break;
    case 2:
        handleLogin();
        break;
    case 0:
        return;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}

void TaskManager::handleSignUp()
{
    std::string username, email, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter email: ";
    std::getline(std::cin, email);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (db.createUser(username, email, password))
    {
        std::cout << "User registered successfully!\n";
    }
    else
    {
        std::cout << "Registration failed.\n";
    }
}

void TaskManager::handleLogin()
{
    std::string username, password;
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    User user;
    if (db.loginUser(username, password, user))
    {
        currentUser = new User(user);
        std::cout << "Login successful!\n";
        handleGroupMenu();
    }
    else
    {
        std::cout << "Invalid username or password.\n";
    }
}

void TaskManager::handleGroupMenu()
{
    int choice;
    std::cout << "1. Create a new group\n2. View groups\n0. Return to Menu\n";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice)
    {
    case 1:
        createGroup();
        break;
    case 2:
        viewGroups();
        break;
    case 0:
        return;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}

void TaskManager::createGroup()
{
    std::string groupName;
    std::cout << "Enter group name: ";
    std::getline(std::cin, groupName);

    if (db.createGroup(groupName, currentUser->getUserId()))
    {
        std::cout << "Group created successfully!\n";
    }
    else
    {
        std::cout << "Failed to create group.\n";
    }
}

void TaskManager::viewGroups()
{
    std::vector<Group> groups = db.getUserGroups(currentUser->getUserId());
    if (groups.empty())
    {
        std::cout << "No groups found.\n";
        return;
    }

    for (size_t i = 0; i < groups.size(); ++i)
    {
        std::cout << (i + 1) << ". " << groups[i].getGroupName() << "\n";
    }

    int groupChoice;
    std::cout << "Select a group: ";
    std::cin >> groupChoice;
    std::cin.ignore();

    if (groupChoice >= 1 && groupChoice <= groups.size())
    {
        handleTaskMenu(groups[groupChoice - 1].getGroupId());
    }
    else
    {
        std::cout << "Invalid choice.\n";
    }
}

void TaskManager::handleTaskMenu(int groupId)
{
    int choice;
    std::cout << "1. Create a new task\n2. Mark task as complete\n0. Return to Menu\n";
    std::cin >> choice;
    std::cin.ignore();

    switch (choice)
    {
    case 1:
        createTask(groupId);
        break;
    case 2:
        completeTask(groupId);
        break;
    case 0:
        return;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}

void TaskManager::createTask(int groupId)
{
    std::string taskName, description, dueDate;
    int priority;
    std::cout << "Enter task name: ";
    std::getline(std::cin, taskName);
    std::cout << "Enter task description: ";
    std::getline(std::cin, description);
    std::cout << "Enter task due date: ";
    std::getline(std::cin, dueDate);
    std::cout << "Enter task priority (1-3): ";
    std::cin >> priority;
    std::cin.ignore();

    if (db.createTask(taskName, description, dueDate, priority, groupId, currentUser->getUserId()))
    {
        std::cout << "Task created successfully!\n";
    }
    else
    {
        std::cout << "Failed to create task.\n";
    }
}

void TaskManager::completeTask(int groupId)
{
    std::vector<Task> tasks = db.getGroupTasks(groupId);
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        std::cout << (i + 1) << ". " << tasks[i].getTaskName() << "\n";
    }

    int taskChoice;
    std::cout << "Select a task to mark as complete: ";
    std::cin >> taskChoice;
    std::cin.ignore();

    if (taskChoice >= 1 && taskChoice <= tasks.size())
    {
        if (db.completeTask(tasks[taskChoice - 1].getTaskId()))
        {
            std::cout << "Task marked as complete.\n";
        }
        else
        {
            std::cout << "Failed to mark task as complete.\n";
        }
    }
    else
    {
        std::cout << "Invalid choice.\n";
    }
}