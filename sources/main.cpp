#include "../headers/Database.h"
#include "../headers/User.h"
#include "../headers/Color.h"
#include "../headers/Timer.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <windows.h>

void setconsolecolor(int textColor, int bgColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}

// Creating a group
void createGroup(Database &db, User &user)
{
    std::cout << "Enter group name: ";
    std::string groupName;
    std::getline(std::cin, groupName);

    if (db.createGroup(groupName, user.getUserId()))
    {
        std::cout << "Group created successfully!\n";
    }
    else
    {
        std::cout << "Failed to create group.\n";
    }
    return;
}

void showTasks(int groupNumber, Database &db, std::vector<Group> &groups);
void createTask(int groupNumber, User &user, Database &db, std::vector<Group> &groups);
void completeTask(std::vector<Group> groups, int groupNumber, Database &db);
void taskMenu(int groupNumber, User &user, Database &db, std::vector<Group> &groups);

// Viewing Groups
void viewGroups(Database &db, User &user)
{
    std::vector<Group> groups = db.getUserGroups(user.getUserId());

    if (groups.empty())
    {
        std::cout << "No groups found.\n";
    }
    else
    {
        SetColor(94);
        std::cout << "Your groups:\n";
        ResetColor();
        int i = 1;
        for (const auto &group : groups)
        {
            std::cout << i << ". " << group.getGroupName() << std::endl;
            i++;
        }
    }

    std::cout << "Select a group: ";
    int groupNumber;
    std::cin >> groupNumber;
    std::cin.ignore();
    std::cout << std::endl;

    // Display tasks for the selected group
    if (groupNumber >= 1 && groupNumber <= groups.size())
    {
        // showTasks(groupNumber, db, groups);
    }
    else
    {
        std::cout << "Invalid group number.\n";
        return;
    }

    int choice1 = 1;
    do
    {
        std::cout << "1. View Tasks\n2. View Members\n0. Return to Menu\n";
        std::cout << "----------------\n";
        std::cout << "Enter choice: ";
        std::cin >> choice1;
        std::cin.ignore();
        std::cout << std::endl;

        // Open Task Menu
        if (choice1 == 1)
        {
            taskMenu(groupNumber, user, db, groups);
        }

        // View members of the group
        else if (choice1 == 2)
        {
            std::cout << "Viewing members of " << groups[groupNumber - 1].getGroupName() << ":\n";
            std::cout << "----------------\n";
            std::vector<User> members = db.getGroupMembers(groups[groupNumber - 1].getGroupId());
            if (members.empty())
            {
                std::cout << "No members found.\n";
            }
            else
            {
                int i = 1;
                for (const auto &member : members)
                {
                    std::cout << i << ". " << member.getUsername() << std::endl;
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

            if (choice2 == 1)
            {
                std::cout << "Enter member username: ";
                std::string username;
                std::getline(std::cin, username);
                if (db.addGroupMember(groups[groupNumber - 1].getGroupId(), username))
                {
                    groups[groupNumber - 1].addUser(new User(username));
                    std::cout << "Member added successfully!\n";
                }
                else
                {
                    std::cout << "Failed to add member.\n";
                }
            }
            else if (choice2 == 2)
            {
                std::cout << "Enter member username: ";
                std::string username;
                std::getline(std::cin, username);
                if (db.removeGroupMember(groups[groupNumber - 1].getGroupId(), username))
                {
                    std::cout << "Member removed successfully!\n";
                }
                else
                {
                    std::cout << "Failed to remove member.\n";
                }
            }
            else if (choice2 == 0)
            {
                return;
            }
            else
            {
                std::cout << "Invalid choice.\n";
            }
        }
        else
        {
            std::cout << "Invalid choice.\n";
        }
    } while (choice1 != 0);
}

//! TASK FUNCTIONS
// Task menu
void taskMenu(int groupNumber, User &user, Database &db, std::vector<Group> &groups)
{
    showTasks(groupNumber, db, groups);
    std::cout << "\n1. Create a new task\n2. Mark task as complete\n3. Mark task as in progress\n4. Mark task as pending\n0. Return to Menu\n";
    std::cout << "----------------\n";
    std::cout << "Enter choice: ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    std::cout << std::endl;

    while (choice != 0)
    {
        // Create a new task
        if (choice == 1)
        {
            createTask(groupNumber, user, db, groups);
        }

        else if (choice == 2)
        {
            completeTask(groups, groupNumber, db);
        }

        else if (choice == 0)
        {
            return;
        }

        else
        {
            std::cout << "Invalid choice.\n";
        }

        showTasks(groupNumber, db, groups);
        std::cout << "\n1. Create a new task\n2. Mark task as complete\n3. Mark task as in progress\n4. Mark task as pending\n0. Return to Menu\n";
        std::cout << "----------------\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
    }
}

void showTasks(int groupNumber, Database &db, std::vector<Group> &groups)
{
    if (groupNumber >= 1 && groupNumber <= groups.size())
    {
        std::vector<Task> tasks = db.getGroupTasks(groups[groupNumber - 1].getGroupId());

        if (tasks.empty())
        {
            std::cout << "No tasks found.\n";
        }
        else
        {
            std::cout << "Tasks in " << groups[groupNumber - 1].getGroupName() << ":\n";
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
}

void createTask(int groupNumber, User &user, Database &db, std::vector<Group> &groups)
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

    if (db.createTask(taskName, taskDescription, dueTime, taskPriority, groups[groupNumber - 1].getGroupId(), user.getUserId()))
    {
        std::cout << "Task created successfully!\n";
    }
    else
    {
        std::cout << "Failed to create task.\n";
    }
}

void completeTask(std::vector<Group> groups, int groupNumber, Database &db)
{
    std::cout << "Enter the task number to mark as complete: ";
    int taskNumber;
    std::cin >> taskNumber;
    std::cin.ignore();

    // Fetch tasks for the selected group
    std::vector<Task> tasks = db.getGroupTasks(groups[groupNumber - 1].getGroupId());

    std::cout << "Task size:" << tasks.size() << std::endl;

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

//! MAIN
int main()
{
    Database db("database.db");
    setconsolecolor(0, 7);
    std::string username, email, password;
    int choice;
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
    SetColor(0);
    std::cout << "----------------\n";
    std::cout << "Enter choice: ";
    std::cin >> choice;
    std::cin.ignore();

    //! Sign Up
    while (choice != 0)
    {
        if (choice == 1)
        {
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

        //! Log In
        else if (choice == 2)
        {
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            User user;

            if (db.loginUser(username, password, user))
            {
                std::cout << "Login successful!\n";
                std::cout << "\n\nWelcome, ";
                SetColor(92);
                std::cout << user.getUsername() << std::endl;
                ResetColor();

                //! GROUPS MENU

                do
                {
                    std::cout
                        << "1. Create a new group\n2. View groups\n0. Close app\n";
                    std::cout << "----------------\n";
                    std::cout << "Enter choice: ";

                    std::cin >> choice;
                    std::cin.ignore();
                    // Create a new group
                    if (choice == 1)
                    {
                        createGroup(db, user);
                    }

                    // View groups
                    else if (choice == 2)
                    {
                        viewGroups(db, user);
                    }

                    else if (choice == 0)
                    {
                        std::cout << "Goodbye!\n";
                        return 0;
                    }

                    else
                    {
                        std::cout << "Invalid choice.\n";
                    }

                } while (choice != 0);
            }
            else
            {
                std::cout << "Invalid username or password.\n";
            }
        }

        else
        {
            std::cout << "Invalid choice.\n";
        }

        std::cout << "1. Sign Up\n2. Log In\n0. Exit\n";
        std::cout << "----------------\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
    }

    return 0;
}
