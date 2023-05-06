#include "q.h"
#include <string>
#include <fstream>
#include <atomic>
#include <iostream>

Q::Q() : id{0}, to{0}, from{0}, p_qid{0}, q{""}, ans{"NOT ANSWERED"}, is_anonymous{0} {}

Q::Q(unsigned long id, unsigned long p_qid, bool is_anonymous, unsigned long to, unsigned long from, std::string q, std::string ans) : id{id}, p_qid{p_qid}, is_anonymous{is_anonymous}, to{to}, from{from}, q{q}, ans{ans}
{
}

unsigned long Q::get_last_qid()
{
    std::ifstream fin(q_last_id);
    unsigned long l_qid;
    if (!(fin >> l_qid))
        l_qid = 0;
    fin.close();
    return l_qid;
}

int Q::generate_id()
{
    static std::atomic<unsigned long> n_qid = get_last_qid() + 1;

    std::ofstream fout(q_last_id);
    fout << n_qid;
    fout.close();
    id = n_qid++;
    return 0;
}

int Q::print()
{
    if (p_qid)
        std::cout << "\t|\n\t|_ _";
    else
        std::cout << "==>";
    std::cout << "QID [" << id << "]";
    if (p_qid)
        std::cout << "  P_QID [" << p_qid << "]";
    if (!is_anonymous)
        std::cout << "  From [" << from << "]";
    std::cout << "  To [" << to << "]"
              << "  Q: " << q << "\n";
    std::cout << "\tAns: " << ans << "\n";
    return 0;
}

// Getters
const unsigned long &Q::get_id() const
{
    return id;
}

const unsigned long &Q::get_to() const
{
    return to;
}

const unsigned long &Q::get_from() const
{
    return from;
}

const unsigned long &Q::get_p_qid() const
{
    return p_qid;
}

const std::string &Q::get_q() const
{
    return q;
}

const std::string &Q::get_ans() const
{
    return ans;
}

bool Q::get_anonymity() const
{
    return is_anonymous;
}

// Setters

void Q::set_parent_id(unsigned long parent_id)
{
    p_qid = parent_id;
    return;
}

void Q::set_anonymity(bool x)
{
    is_anonymous = x;
}

void Q::set_to(unsigned long to_id)
{
    to = to_id;
}

void Q::set_from(unsigned long from_id)
{
    from = from_id;
}

void Q::set_q(std::string body)
{
    q = body;
}