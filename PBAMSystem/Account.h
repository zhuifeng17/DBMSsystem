#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <iostream>
#include <cmath>
#include <map>
#include <stdexcept>
#include "Date.h"
#include "Accumulator.h"

using namespace std;

class Account;	//forward declaration

class AccountRecord
{
private:
	Date date;
	const Account* account;
	double amount;
	double balance;
	string desc;
public:
	//constructor
	AccountRecord(const Date& date, const Account* account, double amount, double balance, const string& desc);

	void show() const;
};

//store record
typedef multimap<Date, AccountRecord> RecordMap;

class Account
{
private:
	string id;	//account
	double balance;
	static double total;	//sum of account
	static RecordMap recordMap;

protected:
	//constructor
	Account(const Date& date, const string& id);

	//tally
	void record(const Date& date, double amount, const string& desc);

	//report error message
	void error(const string& msg) const;

public:
	const string& getId() const { return id; }
	double getBalance() const { return balance; }
	static double getTotal() { return total; }
	
	//deposit cash
	virtual void deposit(const Date& date, double amount, const string& desc) = 0;

	//withdraw cash
	virtual void withdraw(const Date& date, double amount, const string& desc) = 0;

	//settle interest, fee
	virtual void settle(const Date& date) = 0;

	//show account information
	virtual void show(ostream &out) const;

	//query record
	static void query(const Date& begin, const Date& end);
};


//Saving account
class SavingsAccount :public Account	
{
private:
	double rate;	//year rate
	Accumulator acc;	//to assist calculate interest

public:
	//constructor
	SavingsAccount(const Date& date, const string& id, double rate);

	double getRate() const { return rate; }

	//deposit cash
	virtual void deposit(const Date& date, double amount, const string& desc);
	
	//withdraw cash
	virtual void withdraw(const Date& date, double amount, const string& desc);

	//settle interest
	virtual void settle(const Date& date);

};

inline ostream& operator <<(ostream& out, const Account& account)
{
	account.show(out);
	return out;
}

//credit card account
class CreditAccount :public Account
{
private:
	Accumulator acc;	//to assist calculate interest
	double credit;	//credit limit
	double rate;	//debt daily rate
	double fee;		//annual fee

	//get debt
	double getDebt() const
	{
		double balance = getBalance();
		return (balance < 0 ? balance : 0);
	}

public:
	//constructor
	CreditAccount(const Date& date, const string& id, double credit, double rate, double fee);
	
	double getCredit() const { return credit; }
	double getRate() const { return rate; }
	double getFee() const { return fee; }

	double getAvailableCredit() const
	{
		if (getBalance() < 0)
			return credit + getBalance();
		else
			return credit;
	}

	//deposit cash
	virtual void deposit(const Date& date, double amount, const string& desc);

	//withdraw cash
	virtual void withdraw(const Date& date, double amount, const string& desc);

	//settle interest
	virtual void settle(const Date& date);

	virtual void show(ostream &out) const;
};

class AccountException :public runtime_error
{
private:
	const Account* account;

public:
	AccountException(const Account* account, const string& msg) :runtime_error(msg), account(account){ }

	const Account* getAccount() const { return account; }
};

#endif // !_ACCOUNT_H_