/*
 * askMe.cpp
 *
 *  Created on: Feb 9, 2023
 *      Author: eastking
 */
#include "user.h"
#include <iostream>
#include <atomic>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <limits>
#include <map>

using namespace std;

string users_file = "users.txt";
string questions_file = "questions.txt";
string q_last_id = "q_last_id.txt";

struct User_DB
{
	User search(string username)
	{
		ifstream fin(users_file);

		if (fin.fail())
		{
			cout << "Can't open the file\n";
			User temp;
			return temp;
		}

		string row;
		while (getline(fin, row))
		{
			vector<string> usr;
			istringstream iss(row);

			string cell;
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

	User search(unsigned long uid)
	{
		ifstream fin(users_file);

		if (fin.fail())
		{
			cout << "Can't open the file\n";
			User temp;
			return temp;
		}

		string row;
		while (getline(fin, row))
		{
			vector<string> usr;
			istringstream iss(row);

			string cell;
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

	vector<User> list()
	{
		vector<User> users;
		ifstream fin(users_file);
		if (fin.fail())
		{
			cout << "Couldn't open users DB!!!\n";
			return users;
		}

		string line;
		while (getline(fin, line))
		{
			vector<string> user;
			string col;
			istringstream iss(line);
			while (getline(iss, col, ','))
				user.push_back(col);

			User u(stoul(user[0]), user[1], user[2], user[3], user[4], user[5] == "1");
			users.push_back(u);
		}

		fin.close();
		return users;
	}
};

struct Q
{
	unsigned long id, to, from, p_qid;
	string q, ans;
	bool is_anonymous;

	Q()
	{
		id = to = from = p_qid = 0;
		q = "";
		ans = "NOT ANSWERED";
		is_anonymous = 0;
	}

	Q(unsigned long _id, unsigned long _p_qid, bool _is_anonymous, unsigned long _to, unsigned long _from, string _q, string _ans)
	{
		id = _id;
		p_qid = _p_qid;
		to = _to;
		from = _from;
		is_anonymous = _is_anonymous;
		q = _q;
		ans = _ans;
	}

	unsigned long get_last_qid()
	{
		ifstream fin(q_last_id);
		unsigned long l_qid;
		if (!(fin >> l_qid))
			l_qid = 0;
		fin.close();
		return l_qid;
	}

	int generate_id()
	{
		static atomic<unsigned long> n_qid = get_last_qid() + 1;

		ofstream fout(q_last_id);
		fout << n_qid;
		fout.close();
		id = n_qid++;
		return 0;
	}

	int print()
	{
		if (p_qid)
			cout << "\t|\n\t|_ _";
		else
			cout << "==>";
		cout << "QID [" << id << "]";
		if (p_qid)
			cout << "  P_QID [" << p_qid << "]";
		if (!is_anonymous)
			cout << "  From [" << from << "]";
		cout << "  To [" << to << "]"
			 << "  Q: " << q << "\n";
		cout << "\tAns: " << ans << "\n";
		return 0;
	}
};

struct Q_DB
{
	int create(Q q)
	{
		ofstream fout(questions_file, ios::app);
		if (fout.fail())
		{
			cout << "Couldn't open questions DB\n";
			return 1;
		}
		q.generate_id();
		fout << q.id << "," << q.p_qid << "," << q.is_anonymous << "," << q.to << "," << q.from << "," << q.q << "," << q.ans << "\n";
		fout.close();
		return 0;
	}

	Q search(unsigned long qid)
	{
		ifstream fin(questions_file);

		string line;
		while (getline(fin, line))
		{
			istringstream iss(line);
			vector<string> q;
			string col{};

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

	map<unsigned long, vector<Q>> get_multi_Q(string search_field, unsigned long uid)
	{
		map<unsigned long, vector<Q>> mp;
		int search_index{0};
		if (search_field == "to")
			search_index = 3;
		else if (search_field == "from")
			search_index = 4;
		else
		{
			cout << "You can only search for fields (to, from)!!!\n";
			return mp;
		}

		ifstream fin(questions_file);

		string row;
		while (getline(fin, row))
		{
			vector<string> cols;
			istringstream iss(row);

			string col;
			while (getline(iss, col, ','))
				cols.push_back(col);

			if (stoul(cols[search_index]) == uid)
			{
				Q q(stoul(cols[0]), stoul(cols[1]), cols[2] == "1", stoul(cols[3]), stoul(cols[4]), cols[5], cols[6]);
				if (q.p_qid)
					mp[q.p_qid].push_back(q);
				else
					mp[q.id].push_back(q);
			}
		}

		fin.close();
		return mp;
	}

	int update(unsigned int qid, string ans)
	{
		string new_file = "updated_questions.txt";

		ofstream fout(new_file);
		if (fout.fail())
		{
			cout << "Couldn't create a new file in DB\n";
			return 1;
		}

		ifstream fin(questions_file);
		if (fin.fail())
		{
			cout << "couldn't open the questions DB\n";
			return 1;
		}

		string line;
		while (getline(fin, line))
		{
			vector<string> cols;
			string col;
			istringstream iss(line);

			while (getline(iss, col, ','))
				cols.push_back(col);

			if (stoul(cols[0]) == qid)
				cols[6] = ans;

			fout << cols[0] << "," << cols[1] << "," << cols[2] << "," << cols[3] << "," << cols[4] << "," << cols[5] << "," << cols[6] << "\n";
		}

		fin.close();
		fout.close();
		int remove_status = remove(questions_file.c_str());
		if (remove_status)
		{
			cout << "Couldn't remove old questions DB\n";
			return 1;
		}

		int rename_status = rename(new_file.c_str(), questions_file.c_str());
		if (rename_status)
		{
			cout << "Couldn't rename the new questions DB\n";
			return 1;
		}
		return 0;
	}

	int Delete(unsigned long qid)
	{
		string new_file = "updated_questions.txt";

		ofstream fout(new_file);
		if (fout.fail())
		{
			cout << "Couldn't create a new file in DB\n";
			return 1;
		}

		ifstream fin(questions_file);
		if (fin.fail())
		{
			cout << "couldn't open the questions DB\n";
			return 1;
		}

		string line;
		while (getline(fin, line))
		{
			vector<string> cols;
			string col;
			istringstream iss(line);

			while (getline(iss, col, ','))
				cols.push_back(col);

			if (stoul(cols[0]) == qid)
				continue;

			fout << cols[0] << "," << cols[1] << "," << cols[2] << "," << cols[3] << "," << cols[4] << "," << cols[5] << "," << cols[6] << "\n";
		}

		fin.close();
		fout.close();
		int remove_status = remove(questions_file.c_str());
		if (remove_status)
		{
			cout << "Couldn't remove old questions DB\n";
			return 1;
		}

		int rename_status = rename(new_file.c_str(), questions_file.c_str());
		if (rename_status)
		{
			cout << "Couldn't rename the new questions DB\n";
			return 1;
		}
		return 0;
	}

	map<unsigned long, vector<Q>> feed(unsigned long uid)
	{
		map<unsigned long, vector<Q>> mp;
		ifstream fin(questions_file);
		if (fin.fail())
		{
			cout << "Couldn't open the File DB!!!!\n";
			return mp;
		}

		string line;
		while (getline(fin, line))
		{
			vector<string> cols;
			istringstream iss(line);
			string col;

			while (getline(iss, col, ','))
				cols.push_back(col);

			if (cols[6] != "NOT ANSWERED" && (stoul(cols[3]) == uid || stoul(cols[4]) == uid))
			{
				Q q(stoul(cols[0]), stoul(cols[1]), cols[2] == "1", stoul(cols[3]), stoul(cols[4]), cols[5], cols[6]);
				if (q.p_qid)
					mp[q.p_qid].push_back(q);
				else
					mp[q.id].push_back(q);
			}
		}

		fin.close();
		return mp;
	}
};

struct AskMe
{
	User logged_user;
	User_DB u_db;
	Q_DB q_db;

	AskMe()
	{
	}

	AskMe(User _user)
	{
		logged_user = _user;
	}

	int menu()
	{
		cout << "Menu\n";
		cout << "\t1: Print Questions To Me\n";
		cout << "\t2: Print Questions From Me\n";
		cout << "\t3: Answer Question\n";
		cout << "\t4: Delete Question\n";
		cout << "\t5: Ask Question\n";
		cout << "\t6: List System Users\n";
		cout << "\t7: Feed\n";
		cout << "\t8: Log Out\n\n";
		int choice{0};
		while (true)
		{
			cout << "Enter a valid choice in range (1 - 8) : ";
			cin >> choice;
			if (1 <= choice && choice <= 8)
				break;
			cout << "Invalid Input\n";
		}
		cout << "*****************************************************\n\n";
		return choice;
	}

	int run()
	{
		while (true)
		{
			int choice = menu();

			if (choice == 1)
				print_q_to_me();

			if (choice == 2)
				print_q_from_me();

			if (choice == 3)
				answer_q();

			if (choice == 4)
				delete_q();

			if (choice == 5)
				Ask_Q();

			if (choice == 6)
				list_users();

			if (choice == 7)
				feed();

			if (choice == 8)
				break;
		}
		return 0;
	}

	int Ask_Q()
	{

		// Find user or cancel operation
		cout << "Enter User ID or 0 to cancel : ";
		unsigned long uid;
		cin >> uid;
		User to_user;

		while (uid)
		{
			to_user = u_db.search(uid);
			if (to_user.get_id())
				break;

			cout << "User doesn't exist!!!\n";
			cout << "Enter User ID or 0 to cancel : ";
			cin >> uid;
		}

		if (!uid)
			return 0;

		// Set is_anonymous boolean
		bool is_anonymous = 0;
		if (!to_user.get_anonymous())
			cout << "This user doesn't allow anonymous questions your identity will be exposed!!\n";

		while (to_user.get_anonymous())
		{
			cout << "Do you want your question to be anonymous or not enter (1 or 0) : ";
			cin >> is_anonymous;
			if (0 <= is_anonymous && is_anonymous <= 1)
				break;
			cout << "Wrong input!!\n";
		}

		// New or Thread question
		cout << "Enter a Question ID for thread questions or 0 for new questions : ";
		unsigned long p_qid;
		cin >> p_qid;

		while (p_qid)
		{
			Q q = q_db.search(p_qid);
			if (q.id)
				break;
			cout << "There is no such Question with the ID " << p_qid << "\n";
			cout << "Enter a Question ID for thread questions or 0 for new questions : ";
			cin >> p_qid;
		}

		cout << "Enter your Question : ";
		string content{""};
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, content);

		Q q;
		q.p_qid = p_qid;
		q.is_anonymous = is_anonymous;
		q.to = uid;
		q.from = logged_user.get_id();
		q.q = content;

		q_db.create(q);

		return 0;
	}

	int print_q_to_me()
	{
		map<unsigned long, vector<Q>> mp;
		mp = q_db.get_multi_Q("to", logged_user.get_id());

		for (auto p : mp)
		{
			for (auto q : p.second)
			{
				q.print();
			}
			cout << "\n\n";
		}
		return 0;
	}

	int print_q_from_me()
	{
		map<unsigned long, vector<Q>> mp;
		mp = q_db.get_multi_Q("from", logged_user.get_id());

		for (auto p : mp)
		{
			for (auto q : p.second)
			{
				q.print();
			}
			cout << "\n\n";
		}
		return 0;
	}

	int answer_q()
	{
		unsigned long qid;
		Q q;
		while (true)
		{
			cout << "Enter QID or 0 to cancel : ";
			cin >> qid;
			if (!qid)
				return 0;
			q = q_db.search(qid);
			if (q.id == 0)
			{
				cout << "No such question ID\n";
				continue;
			}

			if (q.to == logged_user.get_id())
				break;
			cout << "This question isn't for you to answer!!!\n";
		}
		q.print();

		if (q.ans != "NOT ANSWERED")
			cout << "Warning: Already answered. Answer will be updated!!!\n";
		string ans;
		cout << "Enter answer : ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, ans);
		q_db.update(q.id, ans);
		return 0;
	}

	int delete_q()
	{
		unsigned long qid;
		Q q;
		while (true)
		{
			cout << "Enter QID or 0 to cancel : ";
			cin >> qid;
			if (!qid)
				return 0;
			q = q_db.search(qid);
			if (q.id == 0)
			{
				cout << "No such question ID\n";
				continue;
			}

			if (q.from == logged_user.get_id())
				break;
			cout << "This question isn't from you to delete!!!\n";
		}
		q.print();
		cout << "\n\n";

		q_db.Delete(q.id);

		cout << "the above question was deleted successfully!!!!\n";
		return 0;
	}

	int list_users()
	{
		User_DB u_db;
		vector<User> users = u_db.list();

		for (auto user : users)
			user.print();

		return 0;
	}

	int feed()
	{
		map<unsigned long, vector<Q>> feed;
		feed = q_db.feed(logged_user.get_id());
		for (auto p : feed)
			for (auto q : p.second)
				q.print();

		return 0;
	}
};

struct Program
{
	void run()
	{
		while (true)
		{
			int choice = start_menu();

			if (choice == 1)
				sign_up();

			if (choice == 2)
				login();

			if (choice == 3)
				break;
		}
		cout << "GoodBye!!!\n";
		return;
	}

	int start_menu()
	{
		cout << "\nMenu:\n";
		cout << "\t1: Sign Up\n";
		cout << "\t2: Log In\n";
		cout << "\t3: Exit\n";

		int choice = 0;
		while (choice < 1 || choice > 3)
		{
			cout << "Enter your choice : ";
			cin >> choice;

			if (choice < 1 || choice > 3)
				cout << "Wrong Input\n";
		}
		return choice;
	}

	unsigned long get_last_uid()
	{
		ifstream fin("last_user_id.txt");
		unsigned long l_uid;
		if (!(fin >> l_uid))
			l_uid = 0;
		fin.close();
		return l_uid;
	}

	unsigned long generate_id()
	{
		static atomic<unsigned long> id = get_last_uid() + 1;

		ofstream fout("last_user_id.txt");
		fout << id;
		fout.close();
		return id++;
	}

	unsigned long sign_up()
	{
		cout << "Enter Name (no spaces) : ";
		string name;
		cin >> name;

		cout << "Enter Email : ";
		string email;
		cin >> email;

		cout << "Enter username (no spaces) : ";
		string username;
		cin >> username;

		cout << "Enter password : ";
		string password;
		cin >> password;

		cout << "Allow anonymous or not (1 or 0) : ";
		bool an;
		cin >> an;

		// Generate ID

		unsigned long id = generate_id();

		// Adding user to file
		ofstream fout("users.txt", ios::app);
		fout << id << "," << name << "," << email << "," << username << ","
			 << password << "," << an << "\n";
		fout.close();

		return 0;
	}

	void login()
	{
		User_DB user_db;
		string username, password;
		User usr;

		for (int i = 0; i < 3; i++)
		{
			cout << "Enter username : ";
			cin >> username;

			cout << "Enter password : ";
			cin >> password;

			usr = user_db.search(username);
			if (!usr.get_id())
			{
				cout << "User doesn't exist. You got " << 2 - i << " tries left"
					 << "\n";
				continue;
			}

			if (usr.get_password() != password)
			{
				cout << "Wrong Password. You got " << 2 - i << " tries left"
					 << "\n";
				continue;
			}
			break;
		}

		if (!usr.get_id())
			return;

		AskMe ask(usr);
		ask.run();
		return;
	}
};

int main()
{
	Program program;
	program.run();

	return 0;
}
