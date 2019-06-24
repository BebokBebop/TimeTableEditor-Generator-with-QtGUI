#pragma once
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <random>
#include "AvailabilityTable.h"
#include "ShortTime.h"
#include "Group.h"
#include "Teacher.h"
#include "Subject.h"

class Lesson : public AvailabilityTable
{
private:
	int lessonID;
	ShortTime myTime;
	int myDay;
	int myStartRow;
	int myRows;
	Group* group;
	Teacher* teacher;
	Subject* subject;
	std::list<Lesson*> collidingLessons;
	bool teacherColl;
public:
	Lesson(Group* group, Teacher* teacher, Subject* subject, int ID);

	Lesson(Lesson&& oth);
	
	void setDateAndTime(int day, ShortTime& timeWhen);
	
	void setDayAndRow(int day, int newStartRow);
	
	void getDateAndTime(int &day, ShortTime& timeWhen);
	
	void setTeacherColl(bool toWhat);
	
	bool getTeacherColl();
	
	std::list<Lesson*>& getConflctingLessons();
	
	ShortTime getMyTime();
	
	int getMyRows();
	
	int getMyStartRow();
	
	int getMyDay();
	
	std::string getSubjName();
	
	Teacher* getTeacher();
	
	Group* getGroup();
	
	Subject* getSubject();

	void addIfUnique(Lesson* les);

	void addCollidingLesson(Lesson* les);
	
	void removeCollidingLesson(int id);

	bool checkTeacherAvailabilityConflict();

	void swap(Lesson* oth);

	bool claimRandom();
	
	void reclaimLess();
	
	void blockReclaim();
	
	void unblock();
	
	void cutOutMisfits();
	
	virtual void updAvailability();
	
	void showWhen();

	int getID();
};

