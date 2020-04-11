#ifndef _ACCUMULATOR_H_
#define _ACCUMULATOR_H_

#include "Date.h"

class Accumulator
{
private:
	Date lastDate;
	double value;	//cash
	double sum;

public:
	//constructor
	Accumulator(const Date& date, double value) :lastDate(date), value(value), sum(0)
	{

	}

	//to calculate accumulated value
	double getSum(const Date& date) const
	{
		return sum + value * (date - lastDate);
	}
	//to update lastDate ,value and sum
	void change(const Date& date, double value)
	{
		sum = getSum(date);
		lastDate = date;
		this->value = value;
	}

	//initialize
	void reset(const Date& date, double value)
	{
		lastDate = date;
		this->value = value;
		sum = 0;
	}
	
};

#endif // !_ACCUMULATOR_H_

