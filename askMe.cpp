/*
 * askMe.cpp
 *
 *  Created on: Feb 9, 2023
 *      Author: eastking
 */
#include<iostream>
#include<atomic>
#include<fstream>
#include<limits>
#include<sstream>
#include<vector>

using namespace std;

string users = "users.txt";
string questions_file = "questions.txt";
string q_last_id = "q_last_id.txt";

struct User {
	unsigned long id;
	string name;
	string email;
	string username;
	string password;
	bool allow_anonymous;

	User() {
		id = 0;
		name = email = username = password = "";
		allow_anonymous = false;
	}

	User(unsigned long _id, string _name, string _email, string _username, string _password, bool _allow_anonymous) {
		id = _id;
		name = _name;
		email = _email;
		username = _username;
		password = _password;
		allow_anonymous = _allow_anonymous;
	}

	void print() {
		cout << "ID " << id << " name " << name << " email " << email << " username " << username << " password " << password << " Allow-Anonymous " << allow_anonymous << "\n";
	}
};

struct User_DB {
	User search(string username) {
		ifstream fin(users);

		if (fin.fail()) {
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
			while (getline(iss, cell, ',')) {
				usr.push_back(cell);
			}

			if (usr[3] == username) {
				fin.close();
				User temp {stoul(usr[0]), usr[1], usr[2], usr[3], usr[4], usr[5] == "1"};
				return temp;
			};
		}

		fin.close();
		User temp;
		return temp;
	}
};

struct Q {
	unsigned long id, to, from, p_qid;
	string q, ans;
	bool is_anonymous;

	Q() {
		id = to = from = p_qid = 0;
		q = "";
		ans = "NOT ANSWERED";
		is_anonymous = 0;
	}

	Q(unsigned long _id, unsigned long _p_qid, bool _is_anonymous, unsigned long _to, unsigned long _from, string _q, string _ans) {
		id = _id;
		p_qid = _p_qid;
		to = _to;
		from = _from;
		is_anonymous = _is_anonymous;
		q = _q;
		ans = _ans;
	}

	unsigned long get_last_qid() {
		ifstream fin(q_last_id);
		unsigned long l_qid;
		if (!(fin >> l_qid))
			l_qid = 0;
		fin.close();
		return l_qid;
	}

	int generate_id() {
		static atomic<unsigned long> n_qid = get_last_qid() + 1;

		ofstream fout(q_last_id);
		fout << n_qid;
		fout.close();
		id = n_qid++;
		return 0;
	}
};

struct Q_DB {
	int create(Q q) {
		ofstream fout(questions_file, ios::app);
		if (fout.fail()){
			cout << "Couldn't open questions DB\n";
			return 1;
		}
		q.generate_id();
		fout << q.id << "," << q.p_qid << "," << q.is_anonymous << "," << q.to << "," << q.from << "," << q.q << "," << q.ans << "\n";
		return 0;
	}
};

struct Program
{
	void run() {
		while (true) {
			int choice = start_menu();

			if (choice == 1)
				sign_up();

			if (choice == 2)
				login();

			if (choice == 3)
				break;
		}
		cout << "GoodBye\n";
		return;
	}

	int start_menu() {
		cout << "\nMenu:\n";
		cout << "\t1: Sign Up\n";
		cout << "\t2: Log In\n";
		cout << "\t3: Exit\n";

		int choice = 0;
		while (choice < 1 || choice > 3) {
			cout << "Enter your choice : ";
			cin >> choice;

			if (choice < 1 || choice > 3)
				cout << "Wrong Input\n";
		}
		return choice;
	}

	unsigned long get_last_uid() {
		ifstream fin("last_user_id.txt");
		unsigned long l_uid;
		if (!(fin >> l_uid))
			l_uid = 0;
		fin.close();
		return l_uid;
	}

	unsigned long generate_id() {
		static atomic<unsigned long> id = get_last_uid() + 1;

		ofstream fout("last_user_id.txt");
		fout << id;
		fout.close();
		return id++;
	}

	unsigned long sign_up() {
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

	void login() {
		User_DB user_db;
		string username, password;
		User usr;

		for (int i = 0; i < 3; i++) {
			cout << "Enter username : ";
			cin >> username;

			cout << "Enter password : ";
			cin >> password;

			usr = user_db.search(username);
			if (!usr.id){
				cout << "User doesn't exist. You got " << 2 - i << " tries left"
						 << "\n";
				continue;
			}

			if (usr.password != password){
				cout << "Wrong Password. You got " << 2 - i << " tries left"
						 << "\n";
				continue;
			}
			break;
		}

		if (!usr.id)
			return;
		
		cout << usr.name << " is loged in\n";
		return;
	}
};

int main() {
	// Program program;
	// program.run();
	
	return 0;
}

