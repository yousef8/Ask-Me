#include "user_db.h"
#include "user.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

User User_DB::search(std::string username)
{
    std::ifstream fin(db);

    if (fin.fail())
    {
        std::cout << "Can't open the file\n";
        User temp;
        return temp;
    }

    std::string row;
    while (getline(fin, row))
    {
        std::vector<std::string> usr;
        std::istringstream iss(row);

        std::string cell;
        while (getline(iss, cell, ','))
        {
            usr.push_back(cell);
        }

        if (usr[3] == username)
        {
            fin.close();
            User temp{stoul(usr[0]), usr[1], usr[2], usr[3], usr[4], usr[5] == "1"};
            return temp;
        };
    }

    fin.close();
    User temp;
    return temp;
}

User User_DB::search(unsigned long uid)
{
    std::ifstream fin(db);

    if (fin.fail())
    {
        std::cout << "Can't open the file\n";
        User temp;
        return temp;
    }

    std::string row;
    while (getline(fin, row))
    {
        std::vector<std::string> usr;
        std::istringstream iss(row);

        std::string cell;
        while (getline(iss, cell, ','))
        {
            usr.push_back(cell);
        }

        if (stoul(usr[0]) == uid)
        {
            fin.close();
            User temp{stoul(usr[0]), usr[1], usr[2], usr[3], usr[4], usr[5] == "1"};
            return temp;
        };
    }

    fin.close();
    User temp;
    return temp;
}

std::vector<User> User_DB::list()
{
    std::vector<User> users;
    std::ifstream fin(db);
    if (fin.fail())
    {
        std::cout << "Couldn't open users DB!!!\n";
        return users;
    }

    std::string line;
    while (getline(fin, line))
    {
        std::vector<std::string> user;
        std::string col;
        std::istringstream iss(line);
        while (getline(iss, col, ','))
            user.push_back(col);

        User u(stoul(user[0]), user[1], user[2], user[3], user[4], user[5] == "1");
        users.push_back(u);
    }

    fin.close();
    return users;
}