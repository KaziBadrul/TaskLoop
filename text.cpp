// User.cpp
#include "headers/User.h"
#include <iostream>
User::User(int id, std::string uname, std::string mail, std::string pass)
    : userId(id), username(uname), email(mail), password(pass) {}
void User::login()
{
    std::cout << username << " logged in.\n";
}
void User::logout()
{
    std::cout << username << " logged out.\n";
}

// Admin.cpp
#include "headers/Admin.h"
#include <iostream>
Admin::Admin(int id, std::string uname, std::string mail, std::string pass, int level)
    : User(id, uname, mail, pass), adminLevel(level) {}
void Admin::createGroup(Group &group)
{
    std::cout << "Admin created group: " << group.viewGroupMembers().size() << " members.\n";
}
void Admin::assignTaskToGroup(Task &task, Group &group)
{
    std::cout << "Task assigned to group.\n";
}
std::vector<Group> Admin::viewAllGroups()
{
    return {};
}
void Admin::setTaskPriority(Task &task, PriorityLevel priority)
{
    std::cout << "Priority updated.\n";
}

// NormalUser.cpp
#include "headers/NormalUser.h"
#include <iostream>
NormalUser::NormalUser(int id, std::string uname, std::string mail, std::string pass)
    : User(id, uname, mail, pass) {}
std::vector<Task> NormalUser::viewGroupTasks(Group &group)
{
    return {};
}
void NormalUser::startTaskTimer(Task &task)
{
    std::cout << "Started timer for task.\n";
}
void NormalUser::stopTaskTimer(Task &task)
{
    std::cout << "Stopped timer for task.\n";
}

// Group.cpp
#include "headers/Group.h"
#include <iostream>
Group::Group(int id, std::string name) : groupId(id), groupName(name) {}
void Group::addUser(User *user)
{
    members.push_back(user);
}
void Group::removeUser(User *user)
{
    members.erase(std::remove(members.begin(), members.end(), user), members.end());
}
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

public:
    Group(int id, std::string name);
    void addUser(User *user);
    void removeUser(User *user);
    std::vector<User *> viewGroupMembers();
    std::vector<Task> viewGroupTasks();
};
#endif

// Task.h
#ifndef TASK_H
#define TASK_H
#include <string>
#include "Timer.h"
enum class PriorityLevel
{
    LOW,
    MEDIUM,
    HIGH
};
enum class TaskStatus
{
    PENDING,
    IN_PROGRESS,
    COMPLETED
};
class Task
{
private:
    int taskId;
    std::string taskName;
    std::string description;
    std::string dueDate;
    PriorityLevel priority;
    int groupAssignmentId;
    TaskStatus status;

public:
    Task(int id, std::string name, std::string desc, std::string due, PriorityLevel p, int groupId);
    void markComplete();
    void markInProgress();
    void markPending();
};
#endif

// Timer.h
#ifndef TIMER_H
#define TIMER_H
#include <string>
#include "Task.h"
class Timer
{
private:
    int timerId;
    std::string startTime;
    std::string endTime;
    Task *task;

public:
    Timer(int id, std::string start, std::string end, Task *t);
    void start();
    void stop();
};
#endif

// User.cpp
#include "../headers/User.h"
#include <iostream>
User::User(int id, std::string uname, std::string mail, std::string pass)
    : userId(id), username(uname), email(mail), password(pass) {}
void User::login()
{
    std::cout << username << " logged in.\n";
}
void User::logout()
{
    std::cout << username << " logged out.\n";
}

// Admin.cpp
#include "../headers/Admin.h"
#include <iostream>
Admin::Admin(int id, std::string uname, std::string mail, std::string pass, int level)
    : User(id, uname, mail, pass), adminLevel(level) {}
void Admin::createGroup(Group &group)
{
    std::cout << "Admin created group: " << group.viewGroupMembers().size() << " members.\n";
}
void Admin::assignTaskToGroup(Task &task, Group &group)
{
    std::cout << "Task assigned to group.\n";
}
std::vector<Group> Admin::viewAllGroups()
{
    return {};
}
void Admin::setTaskPriority(Task &task, PriorityLevel priority)
{
    std::cout << "Priority updated.\n";
}

// NormalUser.cpp
#include "../headers/NormalUser.h"
#include <iostream>
NormalUser::NormalUser(int id, std::string uname, std::string mail, std::string pass)
    : User(id, uname, mail, pass) {}
std::vector<Task> NormalUser::viewGroupTasks(Group &group)
{
    return {};
}
void NormalUser::startTaskTimer(Task &task)
{
    std::cout << "Started timer for task.\n";
}
void NormalUser::stopTaskTimer(Task &task)
{
    std::cout << "Stopped timer for task.\n";
}

// Group.cpp
#include "../headers/Group.h"
#include <iostream>
Group::Group(int id, std::string name) : groupId(id), groupName(name) {}
void Group::addUser(User *user)
{
    members.push_back(user);
}
void Group::removeUser(User *user)
{
    members.erase(std::remove(members.begin(), members.end(), user), members.end());
}
std::vector<User *> Group::viewGroupMembers()
{
    return members;
}
std::vector<Task> Group::viewGroupTasks()
{
    return {};
}

// Task.cpp
#include "../headers/Task.h"
#include <iostream>
Task::Task(int id, std::string name, std::string desc, std::string due, PriorityLevel p, int groupId)
    : taskId(id), taskName(name), description(desc), dueDate(due), priority(p), groupAssignmentId(groupId), status(TaskStatus::PENDING) {}
void Task::markComplete()
{
    status = TaskStatus::COMPLETED;
    std::cout << "Task marked as completed.\n";
}
void Task::markInProgress()
{
    status = TaskStatus::IN_PROGRESS;
    std::cout << "Task marked as in progress.\n";
}
void Task::markPending()
{
    status = TaskStatus::PENDING;
    std::cout << "Task marked as pending.\n";
}

// Timer.cpp
#include "../headers/Timer.h"
#include <iostream>
Timer::Timer(int id, std::string start, std::string end, Task *t)
    : timerId(id), startTime(start), endTime(end), task(t) {}
void Timer::start()
{
    std::cout << "Timer started for task.\n";
}
void Timer::stop()
{
    std::cout << "Timer stopped for task.\n";
}

// To build app
// PS E:\TaskLoop> gcc -c sources/sqlite3.c -o build/sqlite3.o -Iinclude
// PS E:\TaskLoop> g++ -c sources/main.cpp -o build/main.o -Iinclude
// PS E:\TaskLoop> g++ build/main.o build/sqlite3.o -o bin/my_program