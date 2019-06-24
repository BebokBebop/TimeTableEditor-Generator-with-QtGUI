#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <list>
#include "ShortTime.h"
#include "Lesson.h"

#define FONT_SIZE 15
#define ROW_HEIGHT 2*FONT_SIZE
#define COLUMN_WIDTH 10*FONT_SIZE
#define COLUMN_GAP 5
#define ROW_GAP 4

class TableDrawer
{
	struct GroupDecorator;
	struct LessonDecorator
	{
		sf::Color subjColors[10] =
		{
			sf::Color(100,100,100),
			sf::Color(255,255,0),
			sf::Color(255,0,255),
			sf::Color(0,255,255),
			sf::Color(0,0,255),
			sf::Color(0,255,0),
			sf::Color(225,150,225),
			sf::Color(150,225,225),
			sf::Color(225,225,150),
			sf::Color(255,150,225),
		};
		int lessonID;
		int teacherID;
		int myDay;
		int myStartRow;
		int myRows;
		int draggedRow;
		bool isSet;
		bool conflictWithAnotherLesson;
		bool conflictWithTeacher;
		std::string subjName;
		std::string teacherName;
		GroupDecorator* myGroup;
		sf::Color color;
		LessonDecorator(Lesson* les, GroupDecorator* par);
	};

	struct GroupDecorator
	{
		int groupID;
		std::vector<int> myColumns;
		std::string groupName;
		std::list<LessonDecorator> lessons;
		std::list<GroupDecorator> descendants;

		GroupDecorator(Group* grp);

		GroupDecorator(const GroupDecorator& oth);

		GroupDecorator(GroupDecorator&& oth);

		bool containsColumnID(int id);
		void getLessonsData(std::vector<Lesson*> les, GroupDecorator* par);
		void getPureDescendants(std::vector<Group*> desc);
	};


	sf::Font font;
	sf::RenderWindow& window;
	std::list<GroupDecorator> groupsToDraw;
	std::list<LessonDecorator*> lessonsToDraw;
	int teachersFilterID;
	sf::Vector2f cursor;
	LessonDecorator* draggedLesson;
	std::string days[5];
	sf::Vector2f lessonDragOffset;


	bool existsIn(int id, std::list<GroupDecorator>& chil);

	void addGroupIfUnique(Group* group);

	void addSeparatelyImpureDescendants(std::vector<Group*>& children);

	void addGroupAndDescendantsRecur(Group* group);

	void addIfUnique(std::vector<int>& uniqueIDs, int id);

	void addUniqueChildrenIDs(std::vector<int>& uniqueIDs, std::list<GroupDecorator>& children);

	int getColumnsPerDay();

	void drawGridBase();

	void drawDayLabels();

	void drawTimeColumn();

	int countPureFamily(GroupDecorator& group);

	void drawGroupLabels(GroupDecorator& group);

	void drawLesson(LessonDecorator* les);

	void drawDraggedLesson();
	
	bool localiseRowGrpColDay(int& clickedDay, int& clickedGroupColumn, int& clickedRow, sf::Vector2f clickedPoint);

	LessonDecorator* findLessonByCoords(int clickedDay, int clickedGroupColumn, int clickedRow);

	int countCollumnsNeeded(GroupDecorator& group);

	void calcGroupCollumnIDs(GroupDecorator& group, int &columnID);

	void calcAllGroupCollumnIDs();

	void addIfUniqueID(std::vector<int>& uniqueIDs, int id);

	void addLessonToList(std::list<GroupDecorator>& cos);

	void createLessonList();

public:
	TableDrawer(sf::RenderWindow& window);

	void clear();

	std::vector<std::string> getCurrentGroups();

	std::vector<int> getCurrentGroupsIDs();

	int getWidth();

	int getHeight();
	
	void setTeacherFilter(int id);
	
	void draw();
	
	void addGroupAndDescendants(Group* group);
	
	std::list<LessonDecorator*>& getLessons();
	
	bool findAndSetDraggedLesson(sf::Vector2f clickedPoint);

	bool setNewPosForDraggedLesson(sf::Vector2f clickedPoint);

	void unsetDragged();

	int getDraggedLessonID();
	
	int getDraggedGroupID();

	int getDraggedLessonDay();
	
	int getDraggedStartRow();

	void setLessonDragOffset(sf::Vector2f toWhat);

	void init();
};