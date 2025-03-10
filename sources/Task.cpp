#include "../headers/Task.h"
#include <iostream>
Task::Task(int id, std::string name, std::string desc, std::string due, PriorityLevel p, int groupId, TaskStatus s, int userId, std::string creator)
    : taskId(id), taskName(name), description(desc), dueDate(due), priority(p), groupAssignmentId(groupId), status(TaskStatus::PENDING), creatorId(userId), creatorName(creator) {}

int Task::getTaskId() const
{
    return taskId;
}
std::string Task::getTaskName() const
{
    return taskName;
}
std::string Task::getDescription() const
{
    return description;
}
std::string Task::getDueDate() const
{
    return dueDate;
}
PriorityLevel Task::getPriority() const
{
    return priority;
}
int Task::getGroupAssignmentId() const
{
    return groupAssignmentId;
}
TaskStatus Task::getStatus() const
{
    return status;
}
int Task::getCreatorId() const
{
    return creatorId;
}
std::string Task::getCreatorName() const
{
    return creatorName;
}
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