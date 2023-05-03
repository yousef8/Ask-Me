#ifndef _user_db_H_
#define _user_db_H_

#include "user.h"
#include <string>
#include <vector>

class User_DB
{
private:
    std::string db{"users_db.txt"};

public:
    User search(std::string username);

    User search(unsigned long uid);

    std::vector<User> list();
};

#endif