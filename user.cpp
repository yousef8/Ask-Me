#include "user.h"
#include <iostream>

User::User()
{
    id = 0;
    name = email = username = password = "";
    allow_anonymous = false;
}

User::User(unsigned long id, std::string name, std::string email, std::string username, std::string password, bool allow_anonymous) : id{id}, name{name}, email{email}, username{username}, password{password}, allow_anonymous{allow_anonymous}
{
}

void User::print()
{

    std::cout << "ID: [" << id << "] name: [" << name << "] email: [" << email << "] username: [" << username << "] Allow-Anonymous: [" << allow_anonymous << "]\n";
}

unsigned long User::get_id() const
{
    return id;
}

bool User::get_anonymous() const
{
    return allow_anonymous;
}

std::string User::get_password() const
{
    return password;
}