#pragma once
#include"ShortTime.h"
#include "AvailabilityTable.h"
#include<vector>

class Lesson;

class Group : public AvailabilityTable
{
protected:
	int groupID;
	std::vector<Group*> vParents;
	std::vector<Group*> vChildren;
	std::string groupName;
	std::vector<Lesson*> myLessons;

public:
	void addLesson(Lesson* les);
	
	void updAvailability();
	
	Group(std::string name, int groupID) : groupName(name), groupID(groupID), AvailabilityTable(true) {};

	Group(std::string name, int groupID, std::vector<Group*> vParents);

	void addChild(Group* gr);
	
	void addParent(Group* gr);

	bool containsDescendantGroupID(int id);

	std::vector<Lesson*>& getLessons();
	
	int getID();
	
	std::string getName();
	
	std::vector<Group*>& getParents();

	std::vector<Group*>& getChildren();
};
