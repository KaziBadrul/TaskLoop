#ifndef USER_H
#define USER_H
#include <string>
#include <vector>
class User
{
protected:
    int userId;
    std::string username;
    std::string email;
    std::string password;

public:
    User() = default;
    User(int id, std::string uname, std::string mail, std::string pass);
    User(std::string uname);
    virtual ~User() = default;
    virtual void login();
    virtual void logout();

    int getUserId() const;
    std::string getUsername() const;
    std::string getEmail() const;
    virtual void showDetails() const;
};
#endif