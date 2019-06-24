#pragma once
#include <vector>
#include "AvailabilityTable.h"
#include "ShortTime.h"
class Lesson;

class Teacher : public AvailabilityTable
{
protected:
	int teacherID;
	std::vector<ShortTime> present[WORK_WEEK];
	std::vector<Lesson*> myLessons;
	std::string teacherName;

	void copyPresent(std::vector<ShortTime> present2[WORK_WEEK]);

	int getTeacherID();

	virtual bool getSet();


public:
	void castPresentToAvailability();
	void addLesson(Lesson* les);

	bool checkIfAvailable(int day, ShortTime start, ShortTime len);

	void updAvailability();

	Teacher(std::vector<ShortTime> present[WORK_WEEK], std::string name, int teacherID);

	friend std::ostream& operator<<(std::ostream& out, const Teacher& o) {
		out << o.teacherID << ". " << o.teacherName;
		return out;
	}

	std::string getName();

	std::vector<ShortTime>* getPresent();

	void showPresent();

	int getID();
};
