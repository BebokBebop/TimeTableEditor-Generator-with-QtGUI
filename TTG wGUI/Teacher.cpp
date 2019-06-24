#include "Teacher.h"

void Teacher::copyPresent(std::vector<ShortTime> present2[WORK_WEEK])
{
	for (int i = 0; i < WORK_WEEK; i++)
	{
		present[i] = present2[i];
	}
}

int Teacher::getTeacherID() {
	return teacherID;
}

bool Teacher::getSet()
{
	return isSet;
}

void Teacher::castPresentToAvailability()
{
	ShortTime start(1500);
	setAvailability(false);

	std::vector<ShortTime>::iterator iter;
	for (int i = 0; i < WORK_WEEK; i++)
	{
		for (iter = present[i].begin(); iter != present[i].end(); iter++)
		{
			//iter zawiera godzinê pocz¹tku obecnoœci
			int startOffset = ShortTime(iter->getTime() - start.getTime()) / ShortTime(15);
			iter++; //teraz zawiera godzinê koñca obecnoœci 
			int endOffset = ShortTime(iter->getTime() - start.getTime()) / ShortTime(15);
			for (int j = startOffset; j < endOffset; j++)
			{
				availabilityTable[i][j] = true;
			}
		}
	}
}

void Teacher::addLesson(Lesson * les)
{
	myLessons.push_back(les);
}

bool Teacher::checkIfAvailable(int day, ShortTime start, ShortTime len)
{
	ShortTime end(start);
	end.addTime(len);
	for (std::vector<ShortTime>::iterator i = present[day].begin(); i < present[day].end(); i++)
	{
		if (start >= *i)
		{
			i++;
			if (end <= *i)
			{
				return true;
			}
		}
		else
			i++;
	}
	return false;
}

Teacher::Teacher(std::vector<ShortTime> present[WORK_WEEK], std::string name, int teacherID)
	: teacherName(name), teacherID(teacherID), AvailabilityTable(false)
{
	copyPresent(present);
	castPresentToAvailability();
	//showAvailability();
}

std::string Teacher::getName() {
	return teacherName;
}

std::vector<ShortTime>* Teacher::getPresent()
{
	return present;
}

void Teacher::showPresent()
{
	std::vector<ShortTime>::iterator iter;
	for (int i = 0; i < 5; i++)
	{
		std::cout << "Day " << i + 1 << '\n';
		for (iter = present[i].begin(); iter != present[i].end(); iter++)
		{
			std::cout << *iter << " - " << *(iter + 1) << '\n';
			iter++;
		}

	}
}

int Teacher::getID()
{
	return teacherID;
}
