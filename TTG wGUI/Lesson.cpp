#pragma once
#include "Lesson.h"
#include "ShortTime.h"

Lesson::Lesson(Group * group, Teacher * teacher, Subject * subject, int ID)
	: group(group), teacher(teacher), subject(subject), AvailabilityTable(true), lessonID(ID), myTime(1500)
{
	myDay = 0;
	myStartRow = 0;
	isSet = false;
	teacherColl = false;
	myRows = subject->subjTime / ShortTime(15);
}

Lesson::Lesson(Lesson && oth) : AvailabilityTable(oth)
{
	lessonID = oth.getID();
	myTime = oth.myTime;
	myDay = oth.myDay;
	myStartRow = oth.myStartRow;
	myRows = oth.myRows;
	group = oth.group;
	teacher = oth.teacher;
	subject = oth.subject;
	collidingLessons.swap(oth.collidingLessons);

	for (int i = 0; i < WORK_WEEK; i++)
	{
		availabilityTable[i] = oth.availabilityTable[i];
		oth.availabilityTable[i] = nullptr;
	}
}

void Lesson::setDateAndTime(int day, ShortTime & timeWhen)
{
	this->myDay = day;
	myTime = timeWhen;
}

void Lesson::setDayAndRow(int day, int newStartRow)
{
	this->myDay = day;
	myStartRow = newStartRow;
	int row = myStartRow;
	myTime = ShortTime(1500);
	for (; row > 0; row--) {
		myTime.addTime(ShortTime(15));
	}
}

void Lesson::getDateAndTime(int & day, ShortTime & timeWhen) {
	day = myDay;
	timeWhen = myTime;
}

void Lesson::setTeacherColl(bool toWhat)
{
	teacherColl = toWhat;
}

bool Lesson::getTeacherColl()
{
	return teacherColl;
}

std::list<Lesson*>& Lesson::getConflctingLessons() {
	return collidingLessons;
}

ShortTime Lesson::getMyTime() {
	return myTime;
}

int Lesson::getMyRows()
{
	return myRows;
}

int Lesson::getMyStartRow()
{
	return myStartRow;
}

int Lesson::getMyDay()
{
	return myDay;
}

std::string Lesson::getSubjName()
{
	return subject->getName();
}

Teacher * Lesson::getTeacher() {
	return teacher;
}

Group * Lesson::getGroup() {
	return group;
}

Subject * Lesson::getSubject() {
	return subject;
}

void Lesson::addIfUnique(Lesson * les)
{
	for (auto& i : collidingLessons)
	{
		if (i->getID() == les->getID())
			return;
	}
	collidingLessons.push_back(les);
}

void Lesson::addCollidingLesson(Lesson * les)
{
	addIfUnique(les);
}

void Lesson::removeCollidingLesson(int id)
{
	for (std::list<Lesson*>::iterator i = collidingLessons.begin(); i != collidingLessons.end(); i++)
	{
		if ((*i)->getID() == id) {
			collidingLessons.erase(i);
			break;
		}
	}
}

bool Lesson::checkTeacherAvailabilityConflict()
{
	return teacher->checkIfAvailable(myDay, myTime, subject->subjTime);
}

void Lesson::swap(Lesson * oth)
{
	int othLessonID = oth->getID();
	ShortTime othTime = oth->myTime;
	int othDay = oth->myDay;
	int othStartRow = oth->myStartRow;
	int othRows = oth->myRows;
	Group* othGroup = oth->group;
	Teacher* othTeacher = oth->teacher;
	Subject* othSubject = oth->subject;
	bool othTeacherColl = oth->teacherColl;
	collidingLessons.swap(oth->collidingLessons);

	oth->lessonID = lessonID;
	oth->myTime = myTime;
	oth->myDay = myDay;
	oth->myStartRow = myStartRow;
	oth->myRows = myRows;
	oth->group = group;
	oth->teacher = teacher;
	oth->subject = subject;
	oth->teacherColl = teacherColl;

	lessonID = othLessonID;
	myTime = othTime;
	myDay = othDay;
	myStartRow = othStartRow;
	myRows = othRows;
	group = othGroup;
	teacher = othTeacher;
	subject = othSubject;
	teacherColl = othTeacherColl;

	int * temp;
	for (int i = 0; i < WORK_WEEK; i++) {
		temp = oth->availabilityTable[i];
		oth->availabilityTable[i] = availabilityTable[i];
		availabilityTable[i] = temp;
	}

	std::vector<Lesson*>::iterator i;
	for (i = oth->group->getLessons().begin(); i != oth->group->getLessons().end(); i++)
	{
		if ((*i)->getID() == lessonID) {
			oth->group->getLessons().erase(i);
			oth->group->getLessons().push_back(std::move(oth));
			break;
		}
	}
	for (i = group->getLessons().begin(); i != group->getLessons().end(); i++)
	{
		if ((*i)->getID() == oth->getID()) {
			group->getLessons().erase(i);
			group->getLessons().push_back(std::move(this));
			break;
		}
	}
}

bool Lesson::claimRandom()
{
	ShortTime start(1500);
	int howManyAvail = 0;
	for (int i = 0; i < WORK_WEEK; i++)
		for (int j = 0; j < howManyRows; j++)
			if (availabilityTable[i][j] == true)
				howManyAvail++;

	if (howManyAvail == 0)
		return false; // nie ma miejsca :/

	int randomAvail = rand() % howManyAvail;

	int day = 0;
	int row = 0;
	//std::cout << std::endl;
	//teacher->showAvailability();
	for (day = 0; day < WORK_WEEK; day++)
	{
		for (row = 0; row < howManyRows; row++)
			if (availabilityTable[day][row] == true)
			{
				if (randomAvail-- == 0) {
					myDay = day; //0 - pon
					myStartRow = row; //0 - start
					teacher->claimAvailability(myDay, row, myRows);
					group->claimAvailability(myDay, row, myRows);
					group->updAvailability();
					myTime = start;
					for (; row > 0; row--) {
						myTime.addTime(ShortTime(15));
					}
					day = WORK_WEEK;
					break;
				}
			}
	}
	//std::cout << std::endl;
	//teacher->showAvailability();
	isSet = true;
	return true;
}

void Lesson::reclaimLess()
{
	if (isSet) {
		isSet = false;
		teacher->reclaim(myDay, myStartRow, myRows);
		group->reclaim(myDay, myStartRow, myRows);
		//for (int i = 0; i < WORK_WEEK; i++)
		//	for (int j = 0; j < howManyRows; j++)
		//		if (availabilityTable[i][j] != -1)
		//			availabilityTable[i][j] == true;
		//updAvailability();
		myDay = 0;
		myStartRow = 0;
	}
}

void Lesson::blockReclaim()
{
	//std::cout << *(getTeacher());
	//std::cout << ": " <<subject->subjectName << std::endl;
	//showAvailability();
	isSet = false;
	teacher->reclaim(myDay, myStartRow, myRows);
	group->reclaim(myDay, myStartRow, myRows);
	availabilityTable[myDay][myStartRow] = -1;
	myDay = -1;
	myStartRow = -1;

	//wszystko inne na true, zeby potem cutOutMisfits()
	for (int i = 0; i < WORK_WEEK; i++)
		for (int j = 0; j < howManyRows; j++)
			if (availabilityTable[i][j] != -1) // ==-1 -> zostaw bana
				availabilityTable[i][j] = true;

	//std::cout << '\n';
	//std::cout << '\n';
	updAvailability();
	//showAvailability();
}

void Lesson::unblock()
{
	isSet = false;
	setAvailability(true);
}

void Lesson::cutOutMisfits()
{
	int myRows = subject->subjTime / ShortTime(15);

	for (int i = 0; i < WORK_WEEK; i++)
		for (int j = 0; j < howManyRows; j++) {
			if (availabilityTable[i][j] == true && j + myRows < howManyRows)
			{
				for (int k = 1; k < myRows; k++)
				{
					if (availabilityTable[i][j + k] == false)
					{
						availabilityTable[i][j] = false;
						break;
					}
				}
			}
			else
			{
				if (availabilityTable[i][j] == true)
					availabilityTable[i][j] = false;
			}
		}


}

void Lesson::updAvailability() {
	group->updAvailToArgAND(availabilityTable);
	group->updAvailability();
	teacher->updAvailToArgAND(availabilityTable);
}

void Lesson::showWhen()
{
	switch (myDay) {
	case 0: {
		std::cout << "Poniedzialek: ";
		break;
	}
	case 1: {
		std::cout << "Wtorek: ";
		break;
	}
	case 2: {
		std::cout << "Sroda: ";
		break;
	}
	case 3: {
		std::cout << "Czwartek: ";
		break;
	}
	case 4: {
		std::cout << "Piatek: ";
		break;
	}
	default: {
		std::cout << "wtf - dayUnknown: ";
		break;
	}
	}

	ShortTime meEnd(myTime);

	meEnd.addTime(subject->subjTime);

	std::cout << myTime << " - " << meEnd << std::endl;


	std::cout << std::endl;
}

int Lesson::getID()
{
	return lessonID;
}

void Teacher::updAvailability()
{
	for (auto i : myLessons)
	{
		if (i->getSet())
			i->claimAvailabilityFromArg(availabilityTable);
	}
}

void Group::updAvailability() {
	for (auto i : myLessons)
	{
		if (i->getSet())
			i->updAvailFromArgAND(availabilityTable);
	}

	for (auto i : vParents)
	{
		i->updAvailFromArgAND(availabilityTable);
	}

	for (auto i : vChildren)
	{
		i->updAvailFromArgAND(availabilityTable);
	}
}
