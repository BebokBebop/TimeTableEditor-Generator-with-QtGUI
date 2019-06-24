#pragma once
#include "ShortTime.h"


class Subject
{
protected:
	int subjID;
	std::string subjectName;
public:
	ShortTime subjTime;

	Subject(std::string name, int subjID, int hour, int minute) : subjectName(name), subjID(subjID), subjTime(hour, minute) {};

	int getID()
	{
		return subjID;
	}

	std::string getName()
	{
		return subjectName;
	}
};
