#ifndef _q_db_H_
#define _q_db_H_

#include "q.h"
#include <string>
#include <vector>
#include <map>

class Q_DB
{
private:
    std::string questions_db = "questions.txt";

public:
    int create(Q q);
    Q search(unsigned long qid);
    std::map<unsigned long, std::vector<Q>> get_multi_Q(std::string search_field, unsigned long uid);
    int update(unsigned int qid, std::string ans);
    int Delete(unsigned long qid);
    std::map<unsigned long, std::vector<Q>> feed(unsigned long uid);
};

#endif