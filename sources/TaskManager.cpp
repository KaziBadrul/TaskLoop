#include "../headers/TaskManager.h"
#include <iostream>
#include <conio.h>

std::string getPassword()
{
    std::string password;
    char ch;

    // we take input until user hits enter which is '\r'
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        { // '\b' is backspace

            if (!password.empty())
            {
                std::cout << "\b \b";
                // erase last *
                // \b moves the cursor one step back.
                // the space erases the * by overwriting it.
                // Another \b moves the cursor back again to the correct position.

                password.pop_back();
            }
        }
        else
        {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
}

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
    password = getPassword();

    if (db.createUser(username, email, password))
    {
        std::cout << "User registered successfully! \n";
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
    password = getPassword();

    User user;
    // passing in the user as a reference
    // it changes the users data within the loginUser func
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
    do
    {
        SetColor(94);
        std::cout << "1. ";
        ResetColor();
        std::cout << "Create a new group\n";
        SetColor(94);
        std::cout << "2. ";
        ResetColor();
        std::cout << "View groups\n";
        SetColor(94);
        std::cout << "0. ";
        ResetColor();
        std::cout << "Log Out\n";
        std::cout << "Enter choice: ";
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
            std::cout << "Goodbye :)" << std::endl;
            return;
        default:
            std::cout << "Invalid choice.\n";
            break;
        }
    } while (choice != 0);
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
    // std::cout << "Debug: Loaded " << groups.size() << " groups.\n";
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
    std::cout << "Select a group (0 to cancel): ";
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

void TaskManager::handleSelectedGroupMenu(int groupId, Group &group)
{
    int choice1;

    do
    {
        std::cout << "Viewing group: ";
        SetColor(92);
        std::cout << group.getGroupName() << std::endl;
        ResetColor();

        std::cout << "1. View Tasks\n2. View Members\n0. Return to Main Menu\n";
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

    } while (choice1 != 0);
};

void TaskManager::handleMemberMenu(Group &group, int groupId)
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
            if (member.getUserId() == group.getOwnerId())
            {
                SetColor(94);
                std::cout << member.getUsername() << " (Admin)" << std::endl;
                ResetColor();
                i++;
                continue;
            }
            std::cout << member.getUsername() << std::endl;
            i++;
        }
    }

    // std::cout << "GROUP OWNER: " << group.getOwnerId() << std::endl;
    // std::cout << "CURRENT USER: " << currentUser->getUserId() << std::endl;
    if (group.getOwnerId() == currentUser->getUserId())
    {
        Admin admin(currentUser->getUserId(), currentUser->getUsername(), currentUser->getEmail(), "xxx");
        SetColor(94);
        std::cout << "You are the admin of this group.\n";
        ResetColor();
        handleAdminMenu(admin, group, members);
    }
    else
    {
        std::cout << "You are a regular member of this group.\n";
        std::cout << "0. Return to Menu\n";
        return;
    }
}

void TaskManager::handleAdminMenu(Admin &admin, Group &group, std::vector<User> members)
{
    int groupId = group.getGroupId();
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
        addMember(admin, group, groupId);
        break;
    case 2:
        removeMember(admin, group, groupId);
        break;
    case 0:
        return;
    default:
        std::cout << "Invalid choice.\n";
        break;
    }
}

void TaskManager::addMember(Admin &admin, Group &group, int groupId)
{
    admin.addMember(db, group);
}

void TaskManager::removeMember(Admin &admin, Group &group, int groupId)
{
    admin.removeMember(db, group);
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

            // Priority Printing
            std::cout << "Priority: ";
            if (task.getPriority() == PriorityLevel::URGENT)
            {
                SetColor(91);
                std::cout << "Urgent" << std::endl;
                ResetColor();
            }
            else if (task.getPriority() == PriorityLevel::IMPORTANT)
            {
                SetColor(93);
                std::cout << "Important" << std::endl;
                ResetColor();
            }
            else
            {
                SetColor(92);
                std::cout << "Normal" << std::endl;
                ResetColor();
            }

            // Status Printing
            std::cout << "Status: " << "Pending" << std::endl;
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
    std::cout << "1. Create a new task\n2. Mark task as complete\n3. Save Task to file\n0. Return to Menu\n";
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
        case 3:
            saveTaskToFile(groupId);
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
            std::string taskName = tasks[taskNumber - 1].getTaskName();

            db.completeTask(taskId);
            std::cout << "Task ";
            SetColor(92);
            std::cout << taskName;
            ResetColor();
            std::cout << " marked as complete.\n";
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

void TaskManager::saveTaskToFile(int groupId)
{
    std::cout << "Enter the filename to save tasks to (type AUTO to save to a file automatically): ";
    std::string filename;
    std::getline(std::cin, filename);
    if (filename == "AUTO")
    {
        std::time_t t = std::time(0); // get time now
        std::tm *now = std::localtime(&t);
        filename = "tasks_" + std::to_string(groupId) + "_" + std::to_string(currentUser->getUserId()) + "_" + std::to_string(now->tm_year + 1900) + '-' + std::to_string(now->tm_mon + 1) + '-' + std::to_string(now->tm_mday) + '_' + std::to_string(now->tm_hour) + '-' + std::to_string(now->tm_min) + '-' + std::to_string(now->tm_sec);
    }
    filename += ".txt";

    std::ofstream file(filename);
    if (file.is_open())
    {
        std::vector<Task> tasks = db.getGroupTasks(groupId);

        if (tasks.empty())
        {
            std::cout << "No tasks found.\n";
        }
        else
        {
            for (const auto &task : tasks)
            {
                file << "Task ID: " << task.getTaskId() << std::endl;
                file << "Task Name: " << task.getTaskName() << std::endl;
                file << "Description: " << task.getDescription() << std::endl;
                file << "Priority: " << static_cast<int>(task.getPriority()) << std::endl;
                file << "Status: " << static_cast<int>(task.getStatus()) << std::endl;
                file << "Due Date: " << task.getDueDate() << std::endl;
                file << std::endl;
            }

            SetColor(92);
            std::cout << "Tasks saved to file successfully.\n";
            ResetColor();
        }
    }
    else
    {
        std::cout << "Unable to open file.\n";
    }

    file.close();
}
