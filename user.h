#ifndef _user_H_
#define _user_H_

#include <string>

class User
{
private:
    unsigned long id;
    std::string name;
    std::string email;
    std::string username;
    std::string password;
    bool allow_anonymous;

public:
    User();
    User(unsigned long id, std::string name, std::string email, std::string username, std::string password, bool allow_anonymous);
    void print();

    // Getters
    unsigned long get_id() const;
    bool get_anonymous() const;
    std::string get_password() const;
};
#endif