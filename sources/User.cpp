#include "../headers/User.h"
#include <iostream>

User::User(std::string uname) : username(uname) {}
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

int User::getUserId() const
{
    return userId;
}
std::string User::getUsername() const
{
    return username;
}
std::string User::getEmail() const
{
    return email;
}
void User::showDetails() const
{
    std::cout << "User ID: " << userId << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Email: " << email << std::endl;
}