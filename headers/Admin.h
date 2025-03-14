#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Group.h"
#include "Task.h"
#include "Database.h"

class Admin : public User
{
public:
    Admin(int id, std::string uname, std::string mail, std::string pass);

    int getId() const;
    void addMember(Database &db, Group &group);
    void removeMember(Database &db, Group &group);
};

#endif