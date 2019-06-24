#pragma once
#include "Group.h"

void Group::addLesson(Lesson * les) {
	myLessons.push_back(les);
}

Group::Group(std::string name, int groupID, std::vector<Group*> vParents)
	: groupName(name), groupID(groupID), AvailabilityTable(true) {
	if (vParents.size() > 0)
	{
		for (auto& i : vParents)
			this->vParents.push_back(i);
	}
}

void Group::addChild(Group * gr) {
	vChildren.push_back(gr);
}

void Group::addParent(Group * gr) {
	vParents.push_back(gr);
}

bool Group::containsDescendantGroupID(int id)
{
	if (groupID == id)
		return true;
	else
	{
		for (auto& i : vChildren)
		{
			if (i->containsDescendantGroupID(id))
				return true;
		}
	}
	return false;
}

std::vector<Lesson*>& Group::getLessons()
{
	return myLessons;
}

int Group::getID()
{
	return groupID;
}

std::string Group::getName()
{
	return groupName;
}

std::vector<Group*>& Group::getParents() {
	return vParents;
}

std::vector<Group*>& Group::getChildren() {
	return vChildren;
}
