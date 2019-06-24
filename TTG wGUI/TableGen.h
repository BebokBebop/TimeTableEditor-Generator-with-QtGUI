#pragma once
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include "Lesson.h"
#include "ShortTime.h"
//#include "settings.h"
#define WORK_WEEK 5
class TableGen
{
	ShortTime start, end;
	ShortTime timeUnit; //%60min = 0, np. 10,15,20 min
	ShortTime *breakBegin, *breakEnd; //mo¿e byæ wiele przerw, wiêc tablice
	ShortTime **breaks;
	int howManyBreaks;
	int howManyRows;

	std::list<Teacher> teachers;
	std::list<Group> groups;
	std::list<Subject> subjects;
	std::list<Lesson> lessons;

	int **timeTable;

	int shortTimeToInt(std::string text);

	void readBreaks(std::ifstream& file);

	void readTeachers(std::ifstream& file, int howManyRows);

	void readSubjects(std::ifstream& file);

	void readGroupsandLessons(std::ifstream& file, int howManyRows);

	void readData(std::ifstream& file);

	void generateTimetableScaffolding();

	bool generateTable();

	void sortLessons();

	bool checkCollision(Lesson* A, Lesson* B);

	void unlockAllLessons();

	void unlockAllTeachers();

	void unlockAllGroups();

public:
	TableGen(ShortTime start, ShortTime end, ShortTime timeUnit, ShortTime *breakBegin, ShortTime *breakEnd, int howManyBreaks);

	TableGen(std::ifstream& file);
	
	TableGen();
	
	void clear();
	
	void saveOutput(std::ofstream& plik);
	
	void presentLessons();

	void read(std::ifstream& file);

	void addSubject(std::string subjectName, int subjTimeHour, int subjTimeMinute);

	void addTeacher(std::string teacherName, std::vector<ShortTime> present[WORK_WEEK]);

	void addLesson(std::string groupName, std::string subjectName, std::string teacherName);
	
	void addGroup(std::string groupName, std::vector<Group*> parents);

	Group* getGroup(int id);
	
	Group* getGroup(std::string name);

	std::vector<std::string> getGroupsNames();

	Subject* getSubject(int id);
	Subject* getSubject(std::string name);

	std::vector<std::string> getSubjectsNames();

	Teacher* getTeacher(int id);
	
	Teacher* getTeacher(std::string name);

	std::vector<std::string> getTeachersNames();

	Lesson* getLesson(int id);

	void reset();

	bool runGenerator();

	bool updateCollisions(Lesson* les);

	void updateLesson(int groupID, int lessonID);
};