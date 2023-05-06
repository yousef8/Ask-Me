#include "q_db.h"
#include "q.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

int Q_DB::create(Q q)
{
    std::ofstream fout(questions_db, std::ios::app);
    if (fout.fail())
    {
        std::cout << "Couldn't open questions DB\n";
        return 1;
    }
    q.generate_id();
    fout << q.get_id() << "," << q.get_p_qid() << "," << q.get_anonymity() << "," << q.get_to() << "," << q.get_from() << "," << q.get_q() << "," << q.get_ans() << "\n";
    fout.close();
    return 0;
}

Q Q_DB::search(unsigned long qid)
{
    std::ifstream fin(questions_db);

    std::string line;
    while (getline(fin, line))
    {
        std::stringstream iss(line);
        std::vector<std::string> q;
        std::string col{};

        // qid, p_qid, is_anonymous, to, from, q, ans
        while (getline(iss, col, ','))
        {
            q.push_back(col);
        }

        if (qid != stoul(q[0]))
            continue;

        fin.close();
        //       qid            p_qid    is_anonymous      to          from       q    ans
        Q temp(stoul(q[0]), stoul(q[1]), q[2] == "1", stoul(q[3]), stoul(q[4]), q[5], q[6]);
        return temp;
    }

    fin.close();
    Q temp;
    return temp;
}

std::map<unsigned long, std::vector<Q>> Q_DB::get_multi_Q(std::string search_field, unsigned long uid)
{
    std::map<unsigned long, std::vector<Q>> mp;
    int search_index{0};
    if (search_field == "to")
        search_index = 3;
    else if (search_field == "from")
        search_index = 4;
    else
    {
        std::cout << "You can only search for fields (to, from)!!!\n";
        return mp;
    }

    std::ifstream fin(questions_db);

    std::string row;
    while (getline(fin, row))
    {
        std::vector<std::string> cols;
        std::istringstream iss(row);

        std::string col;
        while (getline(iss, col, ','))
            cols.push_back(col);

        if (stoul(cols[search_index]) == uid)
        {
            Q q(stoul(cols[0]), stoul(cols[1]), cols[2] == "1", stoul(cols[3]), stoul(cols[4]), cols[5], cols[6]);
            if (q.get_p_qid())
                mp[q.get_p_qid()].push_back(q);
            else
                mp[q.get_id()].push_back(q);
        }
    }

    fin.close();
    return mp;
}

int Q_DB::update(unsigned int qid, std::string ans)
{
    std::string new_file = "updated_questions.txt";

    std::ofstream fout(new_file);
    if (fout.fail())
    {
        std::cout << "Couldn't create a new file in DB\n";
        return 1;
    }

    std::ifstream fin(questions_db);
    if (fin.fail())
    {
        std::cout << "couldn't open the questions DB\n";
        return 1;
    }

    std::string line;
    while (getline(fin, line))
    {
        std::vector<std::string> cols;
        std::string col;
        std::istringstream iss(line);

        while (getline(iss, col, ','))
            cols.push_back(col);

        if (stoul(cols[0]) == qid)
            cols[6] = ans;

        fout << cols[0] << "," << cols[1] << "," << cols[2] << "," << cols[3] << "," << cols[4] << "," << cols[5] << "," << cols[6] << "\n";
    }

    fin.close();
    fout.close();
    int remove_status = remove(questions_db.c_str());
    if (remove_status)
    {
        std::cout << "Couldn't remove old questions DB\n";
        return 1;
    }

    int rename_status = rename(new_file.c_str(), questions_db.c_str());
    if (rename_status)
    {
        std::cout << "Couldn't rename the new questions DB\n";
        return 1;
    }
    return 0;
}

int Q_DB::Delete(unsigned long qid)
{
    std::string new_file = "updated_questions.txt";

    std::ofstream fout(new_file);
    if (fout.fail())
    {
        std::cout << "Couldn't create a new file in DB\n";
        return 1;
    }

    std::ifstream fin(questions_db);
    if (fin.fail())
    {
        std::cout << "couldn't open the questions DB\n";
        return 1;
    }

    std::string line;
    while (getline(fin, line))
    {
        std::vector<std::string> cols;
        std::string col;
        std::istringstream iss(line);

        while (getline(iss, col, ','))
            cols.push_back(col);

        if (stoul(cols[0]) == qid)
            continue;

        fout << cols[0] << "," << cols[1] << "," << cols[2] << "," << cols[3] << "," << cols[4] << "," << cols[5] << "," << cols[6] << "\n";
    }

    fin.close();
    fout.close();
    int remove_status = remove(questions_db.c_str());
    if (remove_status)
    {
        std::cout << "Couldn't remove old questions DB\n";
        return 1;
    }

    int rename_status = rename(new_file.c_str(), questions_db.c_str());
    if (rename_status)
    {
        std::cout << "Couldn't rename the new questions DB\n";
        return 1;
    }
    return 0;
}

std::map<unsigned long, std::vector<Q>> Q_DB::feed(unsigned long uid)
{
    std::map<unsigned long, std::vector<Q>> mp;
    std::ifstream fin(questions_db);
    if (fin.fail())
    {
        std::cout << "Couldn't open the File DB!!!!\n";
        return mp;
    }

    std::string line;
    while (getline(fin, line))
    {
        std::vector<std::string> cols;
        std::istringstream iss(line);
        std::string col;

        while (getline(iss, col, ','))
            cols.push_back(col);

        if (cols[6] != "NOT ANSWERED" && (stoul(cols[3]) == uid || stoul(cols[4]) == uid))
        {
            Q q(stoul(cols[0]), stoul(cols[1]), cols[2] == "1", stoul(cols[3]), stoul(cols[4]), cols[5], cols[6]);
            if (q.get_p_qid())
                mp[q.get_p_qid()].push_back(q);
            else
                mp[q.get_id()].push_back(q);
        }
    }

    fin.close();
    return mp;
}