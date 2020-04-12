#include "Account.h"

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

struct deleter
{
	template <class T>
	void operator() (T* p)
	{
		delete p;
	}
};

//to store account and command
class Controller
{
private:
	Date date;
	vector<Account*> accounts;
	bool end;	//if exit

public:
	Controller(const Date &date):date(date),end(false){ }
	~Controller();

	const Date& getDate() const { return date; }
	bool isEnd() const { return end; }

	bool runCommand(const string& cmdLine);
};

Controller::~Controller()
{
	for_each(accounts.begin(), accounts.end(), deleter());
}

bool Controller::runCommand(const string& cmdLine)
{
	istringstream str(cmdLine);
	char cmd, type;
	int index, day;
	double amount, credit, rate, fee;
	string id, desc;
	Account* account;
	Date date1, date2;
	
	str >> cmd;
	switch (cmd)
	{
		//add account
		case 'a':
			str >> type >> id;
			if (type == 's')
			{
				str >> rate;
				account = new SavingsAccount(date, id, rate);
			}
			else
			{
				str >> credit >> rate >> fee;
				account = new CreditAccount(date, id, credit, rate, fee);
			}
			accounts.push_back(account);
			return true;
		//deposit cash
		case 'd':
			str >> index >> amount;
			getline(str, desc);
			accounts[index]->deposit(date, amount, desc);
			return true;
		//withdraw cash
		case 'w':
			str >> index >> amount;
			getline(str, desc);
			accounts[index]->withdraw(date, amount, desc);
			return true;
		//query all account information
		case 's':
			for (size_t i = 0;i < accounts.size();i++)
			{
				cout << "[" << i << "]" << *accounts[i] << endl;
			}
			return false;
		//change date
		case 'c':
			str >> day;
			if (day < date.getDay())
				cout << "You cannot specify a previous day!";
			else if (day > date.getMaxDay())
				cout << "Invalid day!";
			else
				date = Date(date.getYear(), date.getMonth(), day);
			return true;
		//enter next month
		case 'n':
			if (date.getMonth() == 12)
				date = Date(date.getYear() + 1, 1, 1);
			else
				date = Date(date.getYear(), date.getMonth() + 1, 1);
			for (vector<Account*>::iterator iter = accounts.begin();iter != accounts.end();++iter)
				(*iter)->settle(date);
			return true;

			//query record
		case 'q':
			str >> date1 >> date2;
			Account::query(date1, date2);
			return false;
		case 'e':
			end = true;
			return false;
	}
	cout << "Inavild command: " << cmdLine << endl;
	return false;
}

int main()
{
	Date date(2008, 11, 1);
	Controller controller(date);

	string cmdLine;
	const char* FILE_NAME = "commands.txt";

	ifstream fileIn(FILE_NAME);
	//execute every command
	if (fileIn)
	{
		while (getline(fileIn, cmdLine))
		{
			try 
			{
				controller.runCommand(cmdLine);
			}
			catch (exception & e)
			{
				cout << "Bad line in " << FILE_NAME << ": " << cmdLine << endl;
				cout << "Error: " << e.what() << endl;
				return 1;
			}
		}
		fileIn.close();
	}

	ofstream fileOut(FILE_NAME, ios_base::app);
	cout << "(a)add account (d)deposit (w)withdraw (s)show (c)change day (n)next month (q)query (e)exit" << endl;
	while (!controller.isEnd())
	{
		cout << controller.getDate() << "\tTotal: " << Account::getTotal() << "\tcommand> ";
		string cmdLine;
		getline(cin, cmdLine);
		try
		{
			if (controller.runCommand(cmdLine))
				fileOut << cmdLine << endl;
		}catch (AccountException & e)
		{
			cout << "Error(# " << e.getAccount()->getId() << "): " << e.what() << endl;
		}
		catch (exception & e)
		{
			cout << "Error: " << e.what() << endl;
		}
	}
	return 0;
}