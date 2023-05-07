#ifndef _q_db_H_
#define _q_db_H_

#include "q.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

class Q_DB
{
private:
    std::string db = "questions.txt";
    std::istream &ignoreline(std::ifstream &in, std::ifstream::pos_type &pos);
    std::string getLastLine(std::ifstream &in);
    int getLastID();

public:
    int create(Q q);
    Q search(unsigned long qid);
    std::map<unsigned long, std::vector<Q>> get_multi_Q(std::string search_field, unsigned long uid);
    int update(unsigned int qid, std::string ans);
    int Delete(unsigned long qid);
    std::map<unsigned long, std::vector<Q>> feed(unsigned long uid);
};

#endif