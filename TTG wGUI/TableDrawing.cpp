#include "TableDrawing.h"

TableDrawer::LessonDecorator::LessonDecorator(Lesson * les, GroupDecorator * par) : myGroup(par)
{
	lessonID = les->getID();
	myDay = les->getMyDay();
	myStartRow = les->getMyStartRow();
	myRows = les->getMyRows();
	isSet = les->getSet();
	subjName = les->getSubjName();
	color = subjColors[les->getSubject()->getID() % 10];
	teacherName = les->getTeacher()->getName();
	teacherID = les->getTeacher()->getID();
	conflictWithAnotherLesson = false;
	conflictWithTeacher = false;
}

TableDrawer::GroupDecorator::GroupDecorator(Group * grp)
{
	//columnID = -1;
	groupID = grp->getID();
	groupName = grp->getName();
	getLessonsData(grp->getLessons(), this);
	getPureDescendants(grp->getChildren());
}

TableDrawer::GroupDecorator::GroupDecorator(const GroupDecorator & oth)
{
	groupID = oth.groupID;
	oth.myColumns.size();
}

TableDrawer::GroupDecorator::GroupDecorator(GroupDecorator && oth)
{
	groupID = oth.groupID;
	myColumns.swap(oth.myColumns);
	oth.myColumns.clear();
	groupName = oth.groupName;
	lessons.swap(oth.lessons);
	oth.lessons.clear();
	descendants.swap(oth.descendants);
	oth.descendants.clear();
	for (auto& i : lessons)
	{
		i.myGroup = this;
	}
}

bool TableDrawer::GroupDecorator::containsColumnID(int id)
{
	for (auto& i : myColumns)
		if (i == id)
			return true;
	return false;
}

void TableDrawer::GroupDecorator::getLessonsData(std::vector<Lesson*> les, GroupDecorator * par)
{
	for (auto i : les)
	{
		lessons.push_back({ i, par });
	}
}

void TableDrawer::GroupDecorator::getPureDescendants(std::vector<Group*> desc)
{
	for (auto i : desc)
	{
		if (i->getParents().size() == 1) {
			descendants.push_back(i);
		}
	}
}

bool TableDrawer::existsIn(int id, std::list<GroupDecorator>& chil)
{
	for (auto& i : chil)
	{
		if (i.groupID == id)
			return true;
		if (existsIn(id, i.descendants))
			return true;
	}
	return false;
}

void TableDrawer::addGroupIfUnique(Group * group)
{
	for (auto& i : groupsToDraw)
	{
		if (i.groupID == group->getID())
			return;
		if (existsIn(group->getID(), i.descendants))
			return;
	}
	groupsToDraw.push_back(group);
}

void TableDrawer::addSeparatelyImpureDescendants(std::vector<Group*>& children)
{
	for (auto i : children)
	{
		if (i->getParents().size() > 1)
			addGroupAndDescendantsRecur(i); //mixed
		else
			addSeparatelyImpureDescendants(i->getChildren());
	}
}

void TableDrawer::addGroupAndDescendantsRecur(Group * group)
{
	addGroupIfUnique(group);
	if (group->getChildren().size() > 0)
		addSeparatelyImpureDescendants(group->getChildren());
}

void TableDrawer::addIfUnique(std::vector<int>& uniqueIDs, int id)
{
	for (auto i : uniqueIDs)
		if (i == id)
			return;
	uniqueIDs.push_back(id);
}

void TableDrawer::addUniqueChildrenIDs(std::vector<int>& uniqueIDs, std::list<GroupDecorator>& children)
{
	for (auto& i : children)
	{
		if (i.descendants.size() > 0)
			addUniqueChildrenIDs(uniqueIDs, i.descendants);
		else if (i.lessons.size() > 0)
			addIfUnique(uniqueIDs, i.groupID);
	}
}

int TableDrawer::getColumnsPerDay()
{
	int ret = 0;
	for (auto& i : groupsToDraw)
	{
		ret += countCollumnsNeeded(i);
	}

	return ret;
}

void TableDrawer::drawGridBase()
{
	cursor.x = COLUMN_WIDTH + COLUMN_GAP;
	cursor.y = ROW_HEIGHT * 2 + ROW_GAP * 2;
	sf::RectangleShape shape(sf::Vector2f(COLUMN_WIDTH, ROW_HEIGHT));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineThickness(-1);
	shape.setOutlineColor(sf::Color::Black);
	int columns = getColumnsPerDay();
	if (columns == 0)
		columns = 1;

	for (int i = 0; i < WORK_WEEK; i++) {
		for (int i = 0; i < columns; i++) {
			for (int j = 0; j < 5; j++)
			{
				for (int k = 0; k < 4; k++) {
					shape.setPosition(cursor);
					window.draw(shape);
					cursor.y += ROW_HEIGHT;
				}
				cursor.y += ROW_GAP;
			}
			cursor.y = ROW_HEIGHT * 2 + ROW_GAP * 2;
			cursor.x += COLUMN_WIDTH;
		}
		cursor.x += COLUMN_GAP;
	}
}

void TableDrawer::drawDayLabels() {

	cursor.x = COLUMN_WIDTH + COLUMN_GAP;
	cursor.y = 0;

	int columnSize = getColumnsPerDay();
	if (columnSize == 0)
		columnSize = 1;

	sf::RectangleShape shape(sf::Vector2f(COLUMN_WIDTH * columnSize, ROW_HEIGHT));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineThickness(-1);
	shape.setOutlineColor(sf::Color::Black);

	for (int i = 0; i < WORK_WEEK; i++) {
		shape.setPosition(cursor);
		window.draw(shape);
		cursor.x += COLUMN_WIDTH * columnSize + COLUMN_GAP;
	}

	cursor.x = COLUMN_WIDTH * columnSize + COLUMN_GAP;
	cursor.y = ROW_HEIGHT / 2;

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(FONT_SIZE);
	text.setFillColor(sf::Color::Black);
	for (int i = 0; i < WORK_WEEK; i++) {
		text.setString(days[i]);
		cursor.x =
			COLUMN_WIDTH
			+ (i + 1) * COLUMN_GAP
			+ i * COLUMN_WIDTH * columnSize
			+ (COLUMN_WIDTH * columnSize - text.getLocalBounds().width) / 2;
		cursor.y = text.getLocalBounds().height / 2;
		text.setPosition(cursor);
		window.draw(text);
	}

}

void TableDrawer::drawTimeColumn()
{
	cursor.x = 0;
	cursor.y = ROW_HEIGHT * 2 + 2 * ROW_GAP;
	sf::RectangleShape shape(sf::Vector2f(COLUMN_WIDTH, 4 * ROW_HEIGHT));
	shape.setFillColor(sf::Color::White);
	shape.setOutlineThickness(-1);
	shape.setOutlineColor(sf::Color::Black);
	for (int i = 0; i < 5; i++)
	{
		shape.setPosition(cursor);
		window.draw(shape);
		cursor.y += 4 * ROW_HEIGHT + ROW_GAP;
	}

	ShortTime time(1500);
	std::string str = time.getString();
	str += time.getString();

	sf::Text text(str, font, FONT_SIZE);
	text.setFillColor(sf::Color::Black);

	cursor.x = (COLUMN_WIDTH - text.getLocalBounds().width) / 2;

	cursor.y =
		2 * (ROW_HEIGHT + ROW_GAP)
		+ (4 * ROW_HEIGHT - text.getLocalBounds().height) / 2;

	for (int i = 0; i < 5; i++)
	{
		str = time.getString();
		str += '-';
		time.addTime(60);
		str += time.getString();

		text.setString(str);
		text.setPosition(cursor);
		window.draw(text);
		cursor.y += 4 * ROW_HEIGHT + ROW_GAP;
	}

}

int TableDrawer::countPureFamily(GroupDecorator & group)
{
	int ret = 0;
	if (group.descendants.size() > 1)
		for (auto& i : group.descendants)
			ret += countPureFamily(i);
	else
		ret = 1;

	return ret;
}

void TableDrawer::drawGroupLabels(GroupDecorator & group)
{
	int howManyPureFamilyMemb = countCollumnsNeeded(group);
	int columnSize = getColumnsPerDay();

	if (howManyPureFamilyMemb == 1)
	{
		bool anyLessons = false;
		for (auto& i : lessonsToDraw)
			if (i->myGroup->groupID == group.groupID)
			{
				anyLessons = true;
				break;
			}
		if (!anyLessons)
			return;

		sf::RectangleShape shape(sf::Vector2f(COLUMN_WIDTH, ROW_HEIGHT));
		shape.setFillColor(sf::Color::White);
		shape.setOutlineThickness(-1);
		shape.setOutlineColor(sf::Color::Black);

		cursor.x =
			COLUMN_WIDTH + COLUMN_GAP
			+ *(group.myColumns.begin()) * (COLUMN_WIDTH);

		cursor.y =
			ROW_HEIGHT + ROW_GAP;

		for (int i = 0; i < WORK_WEEK; i++) {
			shape.setPosition(cursor);
			window.draw(shape);
			cursor.x += columnSize * COLUMN_WIDTH + COLUMN_GAP;
		}

		//drawGroupName
		std::string str = group.groupName;
		sf::Text text(str, font, FONT_SIZE);
		text.setFillColor(sf::Color::Black);

		cursor.x =
			COLUMN_WIDTH + COLUMN_GAP
			+ (COLUMN_WIDTH - text.getLocalBounds().width) / 2
			+ *(group.myColumns.begin()) * (COLUMN_WIDTH);

		cursor.y =
			ROW_HEIGHT + ROW_GAP
			+ (ROW_HEIGHT - text.getLocalBounds().height) / 2;

		for (int i = 0; i < WORK_WEEK; i++) {
			text.setPosition(cursor);
			window.draw(text);
			cursor.x += columnSize * COLUMN_WIDTH + COLUMN_GAP;
		}
	}
	if (howManyPureFamilyMemb > 1)
	{
		for (auto& child : group.descendants)
		{
			drawGroupLabels(child);
		}
	}
}

void TableDrawer::drawLesson(LessonDecorator * les)
{
	int howManyPureFamilyMemb = countCollumnsNeeded(*(les->myGroup));
	int columnSize = getColumnsPerDay();

	if (draggedLesson != nullptr)
		if (draggedLesson->lessonID == les->lessonID)
			return;//has to be drawn last

	cursor.x = /*isDragged * lessonDragOffset.x */
		+COLUMN_WIDTH + COLUMN_GAP
		+ les->myDay * (columnSize * COLUMN_WIDTH + COLUMN_GAP)
		+ *(les->myGroup->myColumns.begin()) * (COLUMN_WIDTH);

	cursor.y = /*isDragged * lessonDragOffset.y +*/
		2 * ROW_HEIGHT + 2 * ROW_GAP
		+ les->myStartRow * ROW_HEIGHT
		+ les->myStartRow / 4 * ROW_GAP;

	int lessonRows = les->myRows;
	int lessonStartingRow = les->myStartRow;

	float blockHeight = ROW_HEIGHT * lessonRows;
	for (int j = lessonStartingRow + 1; j < lessonStartingRow + lessonRows; j++)
	{
		if (j % 4 == 0)
			blockHeight += ROW_GAP;
	}

	sf::RectangleShape shape(sf::Vector2f(COLUMN_WIDTH*howManyPureFamilyMemb, blockHeight));
	shape.setFillColor(les->color);
	if (les->conflictWithAnotherLesson) {
		shape.setOutlineThickness(-3);
		if (les->conflictWithTeacher)
			shape.setOutlineColor(sf::Color(255, 0, 191));

		else
			shape.setOutlineColor(sf::Color::Red);

	}
	else if (les->conflictWithTeacher) {
		shape.setOutlineThickness(-3);
		shape.setOutlineColor(sf::Color(178, 0, 255));
	}
	else {
		shape.setOutlineThickness(-1);
		shape.setOutlineColor(sf::Color::Black);
	}
	shape.setPosition(cursor);
	window.draw(shape);

	//draw subjName
	std::string str = les->subjName;
	sf::Text text(str, font, FONT_SIZE);
	text.setFillColor(sf::Color::Black);

	cursor.x += (COLUMN_WIDTH*howManyPureFamilyMemb - text.getLocalBounds().width) / 2;
	cursor.y += blockHeight / 2 - text.getLocalBounds().height;
	text.setPosition(cursor);
	window.draw(text);
	cursor.x -= (COLUMN_WIDTH*howManyPureFamilyMemb - text.getLocalBounds().width) / 2;

	str = les->teacherName;
	text.setString(str);

	cursor.x += (COLUMN_WIDTH*howManyPureFamilyMemb - text.getLocalBounds().width) / 2;
	cursor.y += text.getLocalBounds().height*1.3;
	text.setPosition(cursor);
	window.draw(text);
}

void TableDrawer::drawDraggedLesson()
{
	int columnSize = getColumnsPerDay();

	cursor.x = lessonDragOffset.x
		+ COLUMN_WIDTH + COLUMN_GAP
		+ draggedLesson->myDay * (columnSize * COLUMN_WIDTH + COLUMN_GAP)
		+ *(draggedLesson->myGroup->myColumns.begin()) * (COLUMN_WIDTH);

	cursor.y = lessonDragOffset.y
		+ 2 * ROW_HEIGHT + 2 * ROW_GAP
		+ draggedLesson->myStartRow * ROW_HEIGHT
		+ draggedLesson->myStartRow / 4 * ROW_GAP;

	int lessonRows = draggedLesson->myRows;
	int lessonStartingRow = draggedLesson->myStartRow;

	float blockHeight = ROW_HEIGHT * lessonRows;
	for (int j = lessonStartingRow + 1; j < lessonStartingRow + lessonRows; j++)
	{
		if (j % 4 == 0)
			blockHeight += ROW_GAP;
	}

	sf::RectangleShape shape(sf::Vector2f(COLUMN_WIDTH*draggedLesson->myGroup->myColumns.size(), blockHeight));
	shape.setFillColor(draggedLesson->color);
	shape.setOutlineThickness(-1);
	shape.setOutlineColor(sf::Color::Black);

	shape.setPosition(cursor);
	window.draw(shape);

	//draw subjName
	std::string str = draggedLesson->subjName;
	sf::Text text(str, font, FONT_SIZE);
	text.setFillColor(sf::Color::Black);

	cursor.x += (COLUMN_WIDTH*draggedLesson->myGroup->myColumns.size() - text.getLocalBounds().width) / 2;
	cursor.y += blockHeight / 2 - text.getLocalBounds().height;
	text.setPosition(cursor);
	window.draw(text);
	cursor.x -= (COLUMN_WIDTH*draggedLesson->myGroup->myColumns.size() - text.getLocalBounds().width) / 2;

	str = draggedLesson->teacherName;
	text.setString(str);

	cursor.x += (COLUMN_WIDTH*draggedLesson->myGroup->myColumns.size() - text.getLocalBounds().width) / 2;
	cursor.y += text.getLocalBounds().height*1.3;
	text.setPosition(cursor);
	window.draw(text);
}

bool TableDrawer::localiseRowGrpColDay(int & clickedDay, int & clickedGroupColumn, int & clickedRow, sf::Vector2f clickedPoint)
{
	int clickedColumn = 0;
	int columnSize = getColumnsPerDay();
	if (columnSize == 0)
		return false;
	if (columnSize > 0) {
		clickedPoint.x -= COLUMN_WIDTH + COLUMN_GAP;
		if (clickedPoint.x <= 0)
			return false; //na lewo od planu

		while (clickedPoint.x > 0) {
			for (int i = 0; i < columnSize && clickedPoint.x > 0; i++) {
				clickedPoint.x -= COLUMN_WIDTH;
				if (clickedPoint.x > 0)
				{
					clickedColumn++;
				}
			}
			if (clickedPoint.x > 0) {
				clickedPoint.x -= COLUMN_GAP;
				if (clickedPoint.x <= 0)
					return false; //clicked COLUMN_GAP
			}
		}
	}
	clickedDay = clickedColumn / columnSize;
	clickedGroupColumn = clickedColumn - clickedDay * columnSize;
	clickedPoint.y -= ROW_HEIGHT * 2 + ROW_GAP * 2;
	if (clickedPoint.y <= 0)
		return false; //nad planem

	while (clickedPoint.y > 0) {
		for (int i = 0; i < 4 && clickedPoint.y > 0; i++) {
			clickedPoint.y -= ROW_HEIGHT;
			if (clickedPoint.y > 0)
			{
				clickedRow++;
			}
		}
		if (clickedPoint.y > 0) {
			clickedPoint.y -= ROW_GAP;
			if (clickedPoint.y <= 0);
			//return false; //clicked ROW_GAP przechwytuje bar nad gapem
		}
	}
	return true;
}

TableDrawer::LessonDecorator * TableDrawer::findLessonByCoords(int clickedDay, int clickedGroupColumn, int clickedRow)
{
	LessonDecorator* temp;
	std::list<LessonDecorator*>::reverse_iterator i = lessonsToDraw.rbegin();
	for (; i != lessonsToDraw.rend(); i++)
	{
		if ((*i)->myGroup->containsColumnID(clickedGroupColumn))
			if ((*i)->myDay == clickedDay)
				if (clickedRow >= (*i)->myStartRow && clickedRow < (*i)->myStartRow + (*i)->myRows) {
					return *i;
				}
	}
	return nullptr;
}

int TableDrawer::countCollumnsNeeded(GroupDecorator & group)
{
	int ret = 0;
	for (auto& i : group.descendants)
	{
		ret += countCollumnsNeeded(i);
	}

	//if(group.descendants.size()<=1){
	if (ret == 0) {
		bool anyLessons = false;
		for (auto& i : lessonsToDraw)
			if (i->myGroup->groupID == group.groupID)
			{
				anyLessons = true;
				break;
			}
		if (anyLessons)
		{
			ret++;
		}
	}

	return ret;
}

void TableDrawer::calcGroupCollumnIDs(GroupDecorator & group, int & columnID) {
	{
		int howManyPureFamilyMemb = countCollumnsNeeded(group);
		//int columnSize = getColumnsPerDay();
		bool anyLessons = false;
		for (auto& i : lessonsToDraw)
			if (i->myGroup->groupID == group.groupID)
			{
				anyLessons = true;
				break;
			}
		if (anyLessons)
		{
			for (int i = 0; i < howManyPureFamilyMemb; i++)
			{
				if (!group.containsColumnID(columnID + i))
					group.myColumns.push_back(columnID + i);
			}
			if (howManyPureFamilyMemb == 1)
				if (group.descendants.size() == 0)
				{
					columnID++;
				}
				else
				{
					int check = 0;
					for (auto& i : group.descendants)
					{
						check += countCollumnsNeeded(i);
					}
					if (check == 0)
						columnID++;
				}
		}
		if (howManyPureFamilyMemb > 0)
		{
			for (auto& child : group.descendants)
			{
				calcGroupCollumnIDs(child, columnID);
			}
		}
	}
}

void TableDrawer::calcAllGroupCollumnIDs()
{
	int columnID = 0;
	for (auto& i : groupsToDraw)
	{
		calcGroupCollumnIDs(i, columnID);
	}
}

void TableDrawer::addIfUniqueID(std::vector<int>& uniqueIDs, int id)
{
	for (auto& i : uniqueIDs)
	{
		if (i == id)
			return;
	}
	uniqueIDs.push_back(id);
}

void TableDrawer::addLessonToList(std::list<GroupDecorator>& cos)
{
	bool teacherExists = true;
	for (auto &i : cos)
	{
		for (auto& j : i.lessons)
		{
			if (teachersFilterID == -1 || j.teacherID == teachersFilterID)
				lessonsToDraw.push_back(&j);
		}
		addLessonToList(i.descendants);
	}
}

void TableDrawer::createLessonList()
{
	addLessonToList(groupsToDraw);
	calcAllGroupCollumnIDs();
}

TableDrawer::TableDrawer(sf::RenderWindow & window) : window(window)
{
	draggedLesson = nullptr;
	days[0] = "poniedzialek";
	days[1] = "wtorek";
	days[2] = "sroda";
	days[3] = "czwartek";
	days[4] = "piatek";
	if (!font.loadFromFile("Roboto-Regular.ttf"))
	{
		std::cout << "oh no error";
	}
}

void TableDrawer::clear()
{
	draggedLesson = nullptr;
	lessonsToDraw.clear();
	groupsToDraw.clear();
	//teachersFilterID = -1;
}

std::vector<std::string> TableDrawer::getCurrentGroups()
{
	std::vector<std::string> ret;
	std::vector<int> id;
	bool alreadyThere = false;
	for (auto& i : lessonsToDraw)
	{
		for (auto j : id)
			if (i->myGroup->groupID == j)
				alreadyThere = true;

		if (!alreadyThere)
		{
			id.push_back(i->myGroup->groupID);
			ret.push_back(i->myGroup->groupName);
		}
	}
	return ret;
}

std::vector<int> TableDrawer::getCurrentGroupsIDs()
{
	//std::vector<int> ret;
	//bool alreadyThere = false;
	//for (auto& i : lessonsToDraw)
	//{
	//	for (auto j : ret)
	//		if (i->lessonID == j)
	//			alreadyThere = true;

	//	if (!alreadyThere)
	//	{
	//		ret.push_back(i->myGroup->groupID);
	//	}
	//}
	//return ret;

	std::vector<int> ret;
	for (auto& i : groupsToDraw)
	{
		ret.push_back(i.groupID);
	}
	return ret;


}

int TableDrawer::getWidth()
{
	int colperDay = getColumnsPerDay();
	if (colperDay == 0)
		colperDay = 1;
	return 5 * colperDay * COLUMN_WIDTH + COLUMN_WIDTH + 5 * COLUMN_GAP;
}

int TableDrawer::getHeight()
{
	return 22 * ROW_HEIGHT + 6 * ROW_GAP;
}

void TableDrawer::setTeacherFilter(int id)
{
	teachersFilterID = id;
}

void TableDrawer::draw() {
	cursor = sf::Vector2f(0.f, 0.f);

	drawGridBase();
	drawDayLabels();
	drawTimeColumn();

	int columnID = 0;

	for (auto& i : groupsToDraw) {
		drawGroupLabels(i);
	}
	for (auto& les : lessonsToDraw)
	{
		drawLesson(les);
	}
	if (draggedLesson != nullptr)
		drawDraggedLesson();
}

void TableDrawer::addGroupAndDescendants(Group * group)
{
	bool alreadyExists = false;
	for (auto& i : groupsToDraw)
	{
		if (i.groupID == group->getID()) {
			alreadyExists = true;
			break;
		}
		if (!existsIn(group->getID(), i.descendants))
		{
			alreadyExists = true;
			break;
		}
	}

	if (!alreadyExists)
	{
		addGroupAndDescendantsRecur(group);

		if (lessonsToDraw.size() > 0)
			lessonsToDraw.clear();
		init();
	}
}

std::list<TableDrawer::LessonDecorator*>& TableDrawer::getLessons()
{
	return lessonsToDraw;
}

bool TableDrawer::findAndSetDraggedLesson(sf::Vector2f clickedPoint)
{
	int clickedDay = 0;
	int clickedGroupColumn = 0;
	int clickedRow = 0;

	if (!localiseRowGrpColDay(clickedDay, clickedGroupColumn, clickedRow, clickedPoint))
		return false; //incorrect click

	draggedLesson = findLessonByCoords(clickedDay, clickedGroupColumn, clickedRow);
	if (draggedLesson != nullptr)
	{
		draggedLesson->draggedRow = clickedRow - draggedLesson->myStartRow;
		return true;
	}
	return false;
}

bool TableDrawer::setNewPosForDraggedLesson(sf::Vector2f clickedPoint)
{
	int clickedDay = 0;
	int clickedGroupColumn = 0;
	int clickedRow = 0;

	if (!localiseRowGrpColDay(clickedDay, clickedGroupColumn, clickedRow, clickedPoint))
		return false; //incorrect click
	draggedLesson->myDay = clickedDay;
	draggedLesson->myStartRow = clickedRow - draggedLesson->draggedRow;
	if (draggedLesson->myStartRow < 0)
		draggedLesson->myStartRow = 0;
	if (draggedLesson->myStartRow + draggedLesson->myRows > 20)
		draggedLesson->myStartRow = 20 - draggedLesson->myRows;
}

void TableDrawer::unsetDragged()
{
	draggedLesson = nullptr;
}

int TableDrawer::getDraggedLessonID()
{
	if (draggedLesson == nullptr)
		return -1;
	return draggedLesson->lessonID;
}

int TableDrawer::getDraggedGroupID()
{
	if (draggedLesson == nullptr)
		return -1;
	return draggedLesson->myGroup->groupID;
}

int TableDrawer::getDraggedLessonDay()
{
	if (draggedLesson == nullptr)
		return -1;
	return draggedLesson->myDay;
}

int TableDrawer::getDraggedStartRow()
{
	if (draggedLesson == nullptr)
		return -1;
	return draggedLesson->myStartRow;
}

void TableDrawer::setLessonDragOffset(sf::Vector2f toWhat)
{
	lessonDragOffset = toWhat;
}

void TableDrawer::init()
{
	createLessonList();
}
