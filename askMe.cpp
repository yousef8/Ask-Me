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

using namespace std;

struct Program {
	void run() {
		while (true) {
			int choice = start_menu();

			if (choice == 1)
				sign_up();

			if (choice == 3)
				break;
		}
		cout << "GoodBye\n";
		return;
	}

	int start_menu() {
		cout << "\nMenu:\n";
		cout << "\t1: Sign Up\n";
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

	unsigned int get_last_uid() {
		ifstream fin("last_user_id.txt");
		unsigned int l_uid;
		if (!(fin >> l_uid))
			l_uid = 0;
		fin.close();
		return l_uid;
	}

	unsigned int generate_id() {
		static atomic<unsigned int> id = get_last_uid() + 1;

		ofstream fout("last_user_id.txt");
		fout << id;
		fout.close();
		return id++;
	}

	unsigned int sign_up() {
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

		unsigned int id = generate_id();

		// Adding user to file
		ofstream fout("users.txt", ios::app);
		fout << id << "," << name << "," << email << "," << username << ","
				<< password << "," << an << "\n";
		fout.close();

		return 0;
	}
};

int main() {
	Program program;
	program.run();

	return 0;
}

