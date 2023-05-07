#ifndef _q_H_
#define _q_H_

#include <string>

class Q
{
private:
    unsigned long id, to, from, p_qid;
    std::string q, ans;
    bool is_anonymous;

public:
    Q();

    Q(unsigned long _id, unsigned long _p_qid, bool _is_anonymous, unsigned long _to, unsigned long _from, std::string _q, std::string _ans);

    // Getters
    const unsigned long &get_id() const;
    const unsigned long &get_to() const;
    const unsigned long &get_from() const;
    const unsigned long &get_p_qid() const;
    const std::string &get_q() const;
    const std::string &get_ans() const;
    bool get_anonymity() const;

    // Setters
    void set_parent_id(unsigned long parent_id);
    void set_anonymity(bool s);
    void set_to(unsigned long to_id);
    void set_from(unsigned long from_id);
    void set_q(std::string body);

    int print();
};

#endif