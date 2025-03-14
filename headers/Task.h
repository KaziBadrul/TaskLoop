#ifndef TASK_H
#define TASK_H
#include <string>
#include <ctime>
#include "Timer.h"
enum class PriorityLevel
{
    LOW,
    IMPORTANT,
    URGENT
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
    std::time_t dueDate;
    PriorityLevel priority;
    int groupAssignmentId;
    TaskStatus status;
    int creatorId;
    std::string creatorName;
    Timer *taskTimer;

public:
    Task(int id, std::string name, std::string desc, std::time_t due, PriorityLevel p, int groupId, TaskStatus s, int userId, std::string creator);
    int getTaskId() const;
    std::string getTaskName() const;
    std::string getDescription() const;
    std::string getDueDate() const;
    PriorityLevel getPriority() const;
    int getGroupAssignmentId() const;
    int getCreatorId() const;
    std::string getCreatorName() const;
    TaskStatus getStatus() const;
    std::string getTimeRemaining() const;
    void markComplete();
    void markInProgress();
    void markPending();
};
#endif