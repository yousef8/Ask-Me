#ifndef _user_db_H_
#define _user_db_H_

#include "user.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

class User_DB
{
private:
    std::string db{"users_db.txt"};
    std::istream &ignoreline(std::ifstream &in, std::ifstream::pos_type &pos);
    std::string getLastLine(std::ifstream &in);
    int getLastID();

public:
    void create(std::string name, std::string email, std::string username, std::string password, bool allow_anonymous);
    User search(std::string username);

    User search(unsigned long uid);

    std::vector<User> list();
};

#endif