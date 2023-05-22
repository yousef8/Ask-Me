/*
 * askMe.cpp
 *
 *  Created on: Feb 9, 2023
 *      Author: eastking
 */
#include "user.h"
#include "user_db.h"
#include "q.h"
#include "q_db.h"

#include <iostream>
#include <atomic>
#include <fstream>
#include <limits>
#include <sstream>
#include <vector>
#include <limits>
#include <map>

using namespace std;

class questionController
{
private:
	User logged_user;
	User_DB u_db;
	Q_DB q_db;

public:
	questionController(User user) : logged_user{user}
	{
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

	int begin()
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
			if (q.get_id())
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
		q.set_parent_id(p_qid);
		q.set_anonymity(is_anonymous);
		q.set_to(uid);
		q.set_from(logged_user.get_id());
		q.set_q(content);

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
			if (q.get_id() == 0)
			{
				cout << "No such question ID\n";
				continue;
			}

			if (q.get_to() == logged_user.get_id())
				break;
			cout << "This question isn't for you to answer!!!\n";
		}
		q.print();

		if (q.get_ans() != "NOT ANSWERED")
			cout << "Warning: Already answered. Answer will be updated!!!\n";
		string ans;
		cout << "Enter answer : ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, ans);
		q_db.update(q.get_id(), ans);
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
			if (q.get_id() == 0)
			{
				cout << "No such question ID\n";
				continue;
			}

			if (q.get_from() == logged_user.get_id())
				break;
			cout << "This question isn't from you to delete!!!\n";
		}
		q.print();
		cout << "\n\n";

		q_db.Delete(q.get_id());

		cout << "the above question was deleted successfully!!!!\n";
		return 0;
	}

	int list_users()
	{
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

class userController
{
private:
	User_DB db;

	int menu()
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

		// Adding user to file
		db.create(name, email, username, password, an);

		return 0;
	}

	void login()
	{
		string username, password;
		User usr;

		for (int i = 0; i < 3; i++)
		{
			cout << "Enter username : ";
			cin >> username;

			cout << "Enter password : ";
			cin >> password;

			usr = db.search(username);
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

		questionController controller(usr);
		controller.begin();
		return;
	}

public:
	void begin()
	{
		while (true)
		{
			int choice = menu();

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
};

int main()
{
	userController program;
	program.begin();

	return 0;
}