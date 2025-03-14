#include "../headers/sqlite3.h"
#include "../headers/Database.h"
#include <iostream>

Database::Database(const std::string &dbName)
{
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
    else
    {
        std::cout << "Database connected successfully.\n";
    }
}

Database::~Database()
{
    sqlite3_close(db);
}

//! GROUP FUNCTIONS
// Create a new group
bool Database::createGroup(const std::string &groupName, int ownerId)
{
    std::string sql = "INSERT INTO groups (name, owner_id) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, groupName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, ownerId);

    // std::cout << "Created group: " << groupName << "with Owner Id: " << ownerId << std::endl;

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error creating group: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    int groupId = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);

    std::string insertMemberSql = "INSERT INTO group_members (group_id, user_id) VALUES (?, ?);";

    if (sqlite3_prepare_v2(db, insertMemberSql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement for adding user to group: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, ownerId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error adding owner to group members: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Add user to group
bool Database::addUserToGroup(int groupId, int userId)
{
    std::string sql = "INSERT INTO group_members (group_id, user_id) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, userId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error adding user to group: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Remove user from group
bool Database::removeUserFromGroup(int groupId, int userId)
{
    std::string sql = "DELETE FROM group_members WHERE group_id = ? AND user_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, groupId);
    sqlite3_bind_int(stmt, 2, userId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error removing user from group: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Create a task for a group
bool Database::createTask(const std::string &taskName, const std::string &description, const std::time_t &dueDate, int priority, int groupId, int creatorId)
{
    std::string creatorName;
    std::string userSql = "SELECT username FROM users WHERE id = ?;";
    sqlite3_stmt *userStmt;

    if (sqlite3_prepare_v2(db, userSql.c_str(), -1, &userStmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(userStmt, 1, creatorId);
        if (sqlite3_step(userStmt) == SQLITE_ROW)
        {
            creatorName = reinterpret_cast<const char *>(sqlite3_column_text(userStmt, 0));
        }
        sqlite3_finalize(userStmt);
    }
    else
    {
        std::cerr << "Failed to get creator name: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "INSERT INTO tasks (name, description, due_date, priority, status, group_id, creator_id, creator_name) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, taskName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 3, dueDate);
    sqlite3_bind_int(stmt, 4, priority);
    sqlite3_bind_int(stmt, 5, static_cast<int>(TaskStatus::PENDING));
    sqlite3_bind_int(stmt, 6, groupId);
    sqlite3_bind_int(stmt, 7, creatorId);
    sqlite3_bind_text(stmt, 8, creatorName.c_str(), -1, SQLITE_TRANSIENT); // 🔹 Insert creator name

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error creating task: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Fetch groups for a user
std::vector<Group> Database::getUserGroups(int userId)
{
    std::vector<Group> groups;
    std::string sql = "SELECT g.id, g.name, g.owner_id FROM groups g JOIN group_members gm ON g.id = gm.group_id WHERE gm.user_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return groups;
    }

    sqlite3_bind_int(stmt, 1, userId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int groupId = sqlite3_column_int(stmt, 0);
        // const char *groupNameCStr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string groupName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        int ownerId = sqlite3_column_int(stmt, 2);

        // std::cout << "Group Fetched: " << groupName << std::endl;

        groups.push_back(Group(groupId, groupName, ownerId));
    }

    sqlite3_finalize(stmt);
    return groups;
}

// Fetch tasks for a group
std::vector<Task> Database::getGroupTasks(int groupId)
{
    std::vector<Task> tasks;
    std::string sql = "SELECT id, name, description, due_date, priority, status, creator_id, creator_name FROM tasks WHERE group_id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Error coming from getUserGroups()" << std::endl;
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return tasks;
    }

    sqlite3_bind_int(stmt, 1, groupId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int taskId = sqlite3_column_int(stmt, 0);
        std::string taskName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string description = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::time_t dueDate = sqlite3_column_int64(stmt, 3);
        int priority = sqlite3_column_int(stmt, 4);
        int status = sqlite3_column_int(stmt, 5);
        int creatorId = sqlite3_column_int(stmt, 6);
        std::string creatorName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)); // 🔹 Retrieve creator name

        tasks.push_back(Task(taskId, taskName, description, dueDate, static_cast<PriorityLevel>(priority), groupId, static_cast<TaskStatus>(status), creatorId, creatorName));
    }

    sqlite3_finalize(stmt);
    return tasks;
}

std::vector<User> Database::getGroupMembers(int groupId)
{
    std::vector<User> members;

    // SQL query to fetch users from the group_members table based on groupId
    std::string sql = "SELECT u.id, u.username, u.email, u.password FROM users u "
                      "JOIN group_members gm ON u.id = gm.user_id "
                      "WHERE gm.group_id = ?;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return members; // Return empty vector on error
    }

    // Bind groupId to the query
    sqlite3_bind_int(stmt, 1, groupId);

    // Execute query and process results
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Retrieve user data from query result
        int userId = sqlite3_column_int(stmt, 0);
        const char *username = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *email = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char *password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        // Create User object and add to members list
        members.push_back(User(userId, std::string(username), std::string(email), std::string(password)));
    }

    sqlite3_finalize(stmt); // Clean up statement

    return members; // Return the list of members
}

// Add members to group
bool Database::addGroupMember(int groupId, const std::string &username)
{
    // Get the userId from the username
    std::string sql = "SELECT id FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    int userId = -1;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        userId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    if (userId != -1)
    {
        std::string insertSQL = "INSERT INTO group_members (group_id, user_id) VALUES (?, ?);";
        if (sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, groupId);
        sqlite3_bind_int(stmt, 2, userId);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            std::cerr << "Failed to add user to group: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        std::cerr << "User not found.\n";
    }

    return false;
}

// Remove members from Group
bool Database::removeGroupMember(int groupId, const std::string &username)
{
    // Get the userId from the username
    std::string sql = "SELECT id FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    int userId = -1;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        userId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    // If userId is found, delete from group_members table
    if (userId != -1)
    {
        std::string deleteSQL = "DELETE FROM group_members WHERE group_id = ? AND user_id = ?;";
        if (sqlite3_prepare_v2(db, deleteSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_bind_int(stmt, 1, groupId);
        sqlite3_bind_int(stmt, 2, userId);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            return true;
        }
        else
        {
            std::cerr << "Failed to remove user from group: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    }
    else
    {
        std::cerr << "User not found.\n";
    }

    return false;
}

// Mark a task as completed
// If completed, remove the task from the database
bool Database::completeTask(int taskId)
{
    std::string sql = "DELETE FROM tasks WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, taskId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error completing task: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Update the status of a task
bool Database::updateTaskStatus(int taskId, int newStatus)
{
    std::string sql = "UPDATE tasks SET status = ? WHERE id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, newStatus); // newStatus can be 0 (Pending), 1 (In Progress), or 2 (Completed)
    sqlite3_bind_int(stmt, 2, taskId);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error updating task status: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

//! USER AUTH FUNCTIONS
// Function to create a new user in the database
bool Database::createUser(const std::string &username, const std::string &email, const std::string &password)
{
    std::string sql = "INSERT INTO users (username, email, password) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string hash = bcrypt::generateHash(password);

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, hash.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// We pass the User object by reference to update it with the user details
// fetched from the database
bool Database::loginUser(const std::string &username, const std::string &password, User &user)
{
    std::string sql = "SELECT id, username, email, password FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_STATIC);

    bool loginSuccess = sqlite3_step(stmt) == SQLITE_ROW; // If a row is found, login is successful

    if (loginSuccess)
    {
        int id = sqlite3_column_int(stmt, 0);
        if (!bcrypt::validatePassword(password, reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))))
        {
            std::cerr << "Invalid password.\n";
            return false;
        }
        std::string uname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::string mail = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        std::string pass = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        user = User(id, uname, mail, pass);
    }

    sqlite3_finalize(stmt);
    return loginSuccess;
}
