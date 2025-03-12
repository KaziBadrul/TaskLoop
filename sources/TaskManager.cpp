#include "../headers/TaskManager.h"
#include <iostream>

// Opening the Database
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

    std::string username, email, password;
    int choice;

    do
    {
        setconsolecolor(0, 7);
        SetColor(34);
        std::cout << R"(
       
  _______        _    _                       
 |__   __|      | |  | |                      
    | | __ _ ___| | _| |     ___   ___  _ __  
    | |/ _` / __| |/ / |    / _ \ / _ \| '_ \ 
    | | (_| \__ \   <| |___| (_) | (_) | |_) |
    |_|\__,_|___/_|\_\______\___/ \___/| .__/ 
                                       | |    
                                       |_|    

       )";
        ResetColor();
        SetColor(94);
        std::cout << std::endl;
        std::cout << "1. Sign Up\n2. Log In\n0. Exit\n";
        ResetColor();
        std::cout << "Enter your choice: ";
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
            std::cout << "Goodbye :)\n";
            return;
        default:
            std::cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 0);
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
        std::cout << "\n\nWelcome, ";
        SetColor(92);
        std::cout << user.getUsername() << std::endl;
        ResetColor();
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
    std::cout << "1. Create a new group\n2. View groups\n0. Log Out\n";
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

    SetColor(94);
    std::cout << "Your groups:\n";
    ResetColor();
    for (size_t i = 0; i < groups.size(); ++i)
    {
        SetColor(92);
        std::cout << (i + 1) << ". ";
        ResetColor();
        std::cout << groups[i].getGroupName() << "\n";
    }

    int groupChoice;
    std::cout << "Select a group: ";
    std::cin >> groupChoice;
    std::cin.ignore();

    if (groupChoice >= 1 && groupChoice <= groups.size())
    {
        // handleTaskMenu(groups[groupChoice - 1].getGroupId());
        handleSelectedGroupMenu(groups[groupChoice - 1].getGroupId(), groups[groupChoice - 1]);
    }
    else
    {
        std::cout << "Invalid choice.\n";
    }
}

void TaskManager::handleSelectedGroupMenu(int groupId, Group group)
{
    int choice1;
    std::cout << "Viewing group: ";
    SetColor(92);
    std::cout << group.getGroupName() << std::endl;
    ResetColor();
    std::cout << "1. View Tasks\n2. View Members\n0. Return to Menu\n";
    std::cout << "----------------\n";
    std::cout << "Enter choice: ";
    std::cin >> choice1;
    std::cin.ignore();
    std::cout << std::endl;

    switch (choice1)
    {
    case 1:
        handleTaskMenu(group, groupId);
        break;
    case 2:
        handleMemberMenu(group, groupId);
        break;
    case 0:
        return;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
};

void TaskManager::handleMemberMenu(Group group, int groupId)
{
    SetColor(94);
    std::cout << "Viewing members of ";
    SetColor(92);
    std::cout << group.getGroupName() << ":\n";
    ResetColor;
    std::cout << "----------------\n";
    std::vector<User> members = db.getGroupMembers(group.getGroupId());
    if (members.empty())
    {
        std::cout << "No members found.\n";
    }
    else
    {
        int i = 1;
        for (const auto &member : members)
        {
            SetColor(92);
            std::cout << i << ". ";
            ResetColor();
            std::cout << member.getUsername() << std::endl;
            i++;
        }
    }
    std::cout << "----------------\n";

    std::cout << "1. Add member\n";
    if (!members.empty())
        std::cout << "2. Remove member\n";
    std::cout << "0. Return to Menu\n";
    std::cout << "----------------\n";
    std::cout << "Enter choice: ";
    int choice2;
    std::cin >> choice2;
    std::cin.ignore();

    switch (choice2)
    {
    case 1:
        addMember(group, groupId);
        break;
    case 2:
        removeMember(group, groupId);
        break;
    case 0:
        return;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}

void TaskManager::addMember(Group group, int groupId)
{
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

void TaskManager::removeMember(Group group, int groupId)
{
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

void TaskManager::handleViewTasks(Group group, int groupId)
{
    std::vector<Task> tasks = db.getGroupTasks(groupId);
    if (tasks.empty())
    {
        std::cout << "No tasks found.\n";
    }
    else
    {
        std::cout << "Tasks in " << group.getGroupName() << ":\n";
        int i = 1;
        for (const auto &task : tasks)
        {
            std::cout << std::string(10, '-') << std::endl;
            SetColor(94);
            std::cout << i << ". " << task.getTaskName() << std::endl;
            ResetColor();
            std::cout << "Description: " << task.getDescription() << std::endl;
            std::cout << "Priority: " << static_cast<int>(task.getPriority()) << std::endl;
            std::cout << "Status: " << static_cast<int>(task.getStatus()) << std::endl;
            std::cout << "Creator: " << task.getCreatorName() << std::endl;
            std::cout << "Due Date: " << task.getDueDate() << std::endl;

            std::cout << "Remaining Time: " << task.getTimeRemaining() << std::endl;
            ResetColor();
            std::cout << std::string(10, '-') << std::endl;
            i++;
        }
    }
}

void TaskManager::handleTaskMenu(Group group, int groupId)
{
    handleViewTasks(group, groupId);
    int choice;
    std::cout << "1. Create a new task\n2. Mark task as complete\n0. Return to Menu\n";
    std::cin >> choice;
    std::cin.ignore();

    while (choice != 0)
    {
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
        handleViewTasks(group, groupId);
        std::cout << "1. Create a new task\n2. Mark task as complete\n0. Return to Menu\n";
        std::cin >> choice;
        std::cin.ignore();
    }
}

void TaskManager::createTask(int groupId)
{
    std::cout << "Enter task name: ";
    std::string taskName;
    std::getline(std::cin, taskName);

    std::cout << "Enter task description: ";
    std::string taskDescription;
    std::getline(std::cin, taskDescription);

    std::cout << "Enter task due date (YYYY-MM-DD): ";
    std::string taskDueDate;
    std::getline(std::cin, taskDueDate);
    std::time_t dueTime = stringToTimeT(taskDueDate);
    if (dueTime == std::time_t())
    {
        std::cout << "Invalid date format. Task creation failed.\n";
        return;
    }

    std::cout << "Enter task priority (1-3): ";
    int taskPriority;
    std::cin >> taskPriority;
    std::cin.ignore();

    if (db.createTask(taskName, taskDescription, dueTime, taskPriority, groupId, currentUser->getUserId()))
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
    std::cout << "Enter the task number to mark as complete: ";
    int taskNumber;
    std::cin >> taskNumber;
    std::cin.ignore();

    // Fetch tasks for the selected group
    std::vector<Task> tasks = db.getGroupTasks(groupId);

    // std::cout << "Task size:" << tasks.size() << std::endl;

    if (taskNumber >= 1 && taskNumber <= tasks.size())
    {
        int taskId = tasks[taskNumber - 1].getTaskId();

        // Update the task status to "Completed" (assuming 2 represents "Completed")
        if (db.updateTaskStatus(taskId, 2)) // 2 corresponds to "Completed"
        {
            db.completeTask(taskId);
            std::cout << "Task marked as complete.\n";
        }
        else
        {
            std::cout << "Error updating task status.\n";
        }
    }
    else
    {
        std::cout << "Invalid task number.\n";
    }
}