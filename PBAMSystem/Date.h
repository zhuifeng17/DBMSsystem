#ifndef _DATA_H_
#define _DATA_H_

#include <iostream>
using namespace std;

class Date
{
private:
	int year;
	int month;
	int day;
	int totalDays;	//days from 1.1.1

public:
	//constructor
	Date(int year = 1, int month = 1, int day = 1);
	
	int getYear() const { return year; }
	int getMonth() const { return month; }
	int getDay() const { return day; }
	
	//get the number of days in the month
	//to determine if "day" is valid
	int getMaxDay() const;

	//Is it a leap year
	bool isLeapYear() const
	{
		return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
	}

	//calculate the days from two Date
	int operator-(const Date& date) const
	{
		return totalDays - date.totalDays;
	}

	int operator<(const Date& date) const
	{
		return totalDays < date.totalDays;
	}

};

istream& operator >>(istream& in, Date& date);
ostream& operator <<(ostream& out, const Date& date);

#endif // !_DATA_H_
