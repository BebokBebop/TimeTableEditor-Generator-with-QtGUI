#include "MyCanvas.h"

MyCanvas::MyCanvas(QWidget * Parent, const QPoint & Position, const QSize & Size) :
	QSFMLCanvas(Parent, Position, Size)
{
}

void MyCanvas::ResizeView(sf::View & view)
{
	float aspectRatio = float(getSize().x) / float(getSize().y);
	view.setSize(VIEW_HEIGHT*aspectRatio, VIEW_HEIGHT);
}

void MyCanvas::updateAll()
{
	for (auto& i : tableDrawer.getLessons())
	{
		tableGen.updateCollisions(tableGen.getLesson(i->lessonID));

		if (tableGen.getLesson(i->lessonID)->getConflctingLessons().size() > 0)
			i->conflictWithAnotherLesson = true;
		else
			i->conflictWithAnotherLesson = false;

		if (tableGen.getLesson(i->lessonID)->getTeacherColl())
			i->conflictWithTeacher = true;
		else
			i->conflictWithTeacher = false;
	}
}

void MyCanvas::OnInit()
{
	view.setCenter(tableDrawer.getWidth() / 2, tableDrawer.getHeight() / 2);
	//view.setSize(tableDrawer.getWidth(), tableDrawer.getHeight());
	setView(view);
	ResizeView(view);
}

void MyCanvas::closeEvent(QCloseEvent * close) {
	sf::RenderWindow::close();
}

void MyCanvas::mousePressEvent(QMouseEvent * event) {
	previousMousePosition = mapPixelToCoords(sf::Vector2i(event->x(), event->y()));
	if (event->button() == Qt::MouseButton::LeftButton)
	{
		//std::cout << previousMousePosition.x << " " << previousMousePosition.y << std::endl;
		if (tableDrawer.findAndSetDraggedLesson(previousMousePosition))
		{
			dragging = true;
			tableDrawer.setLessonDragOffset(sf::Vector2f(0.f, 0.f));
		}
	}
	else if (event->button() == Qt::MouseButton::RightButton)
	{
		moving = true;
	}
}

void MyCanvas::mouseReleaseEvent(QMouseEvent * event) {
	if (moving)
	{
		moving = false;
	}
	else if (dragging)
	{
		previousMousePosition = mapPixelToCoords(sf::Vector2i(event->x(), event->y()));
		tableDrawer.setNewPosForDraggedLesson(previousMousePosition);
		tableGen.getLesson(tableDrawer.getDraggedLessonID())
			->setDayAndRow(tableDrawer.getDraggedLessonDay(), tableDrawer.getDraggedStartRow());
		tableGen.updateLesson(tableDrawer.getDraggedGroupID(), tableDrawer.getDraggedLessonID());
		tableDrawer.unsetDragged();

		drawn = false;
		dragging = false;
		updateAll();
	}
}

void MyCanvas::mouseMoveEvent(QMouseEvent * event) {
	if (moving)
	{
		pixelPos = sf::Vector2i(event->x(), event->y());
		mouseMoveDelta = mapPixelToCoords(pixelPos, view);
		mouseMoveDelta -= previousMousePosition;

		view.move(-mouseMoveDelta.x, -mouseMoveDelta.y);

		pixelPos = sf::Mouse::getPosition(*this);
		previousMousePosition = mapPixelToCoords(pixelPos, view);
		drawn = false;
	}
	else if (dragging)
	{
		pixelPos = sf::Mouse::getPosition(*this);
		mouseMoveDelta = mapPixelToCoords(pixelPos, view);
		mouseMoveDelta -= previousMousePosition;
		tableDrawer.setLessonDragOffset(mouseMoveDelta);
		drawn = false;
	}
}

void MyCanvas::addLessonDlg()
{
	AddLesson* cos = new AddLesson(nullptr, tableGen.getGroupsNames(), tableGen.getSubjectsNames(), tableGen.getTeachersNames());
	cos->show();
	connect(cos, &AddLesson::addLesson, this, &MyCanvas::getNewLesson);
}

void MyCanvas::addSubjDlg() {
	AddSubject* cos = new AddSubject(nullptr);
	cos->show();
	connect(cos, &AddSubject::addSubject, this, &MyCanvas::getNewSubject);
}

void MyCanvas::addTeachDlg() {
	AddTeacher* cos = new AddTeacher(nullptr);
	cos->show();
	connect(cos, &AddTeacher::addTeacher, this, &MyCanvas::getNewTeacher);
}

void MyCanvas::addGroupDlg() {
	AddGroup* cos = new AddGroup(nullptr, tableGen.getGroupsNames());
	cos->show();
	connect(cos, &AddGroup::addGroup, this, &MyCanvas::getNewGroup);
}

void MyCanvas::setFilterDlg() {
	SetFilter* cos = new SetFilter(nullptr, tableGen.getGroupsNames(), tableGen.getTeachersNames(), tableDrawer.getCurrentGroups());
	cos->show();
	connect(cos, &SetFilter::setFilter, this, &MyCanvas::setFilters);
}

void MyCanvas::runGenerator() {
	if (!tableGen.runGenerator())
	{
		FailDlg* cos = new FailDlg(nullptr);
		cos->show();
	}
	updateAll();
	reloadFilter();
	drawn = false;
}

void MyCanvas::saveDataDlg() {
	std::ofstream plik("output.csv");
	tableGen.saveOutput(plik);
	plik.close();
}

void MyCanvas::readData()
{
	std::ifstream plik("input.txt");
	if (plik.is_open()) {
		tableGen.read(plik);
		plik.close();
	}
	else
	{
		FailDlg* cos = new FailDlg(nullptr);
		cos->show();
	}
}

void MyCanvas::keyPressEvent(QKeyEvent * event) {
	switch (event->key())
	{
	case Qt::Key::Key_Space:
	{
		ChooseAction* cos = new ChooseAction(nullptr);
		cos->show();
		connect(cos, &ChooseAction::addLesson, this, &MyCanvas::addLessonDlg);
		connect(cos, &ChooseAction::addSubj, this, &MyCanvas::addSubjDlg);
		connect(cos, &ChooseAction::addGrp, this, &MyCanvas::addGroupDlg);
		connect(cos, &ChooseAction::addTeach, this, &MyCanvas::addTeachDlg);
		connect(cos, &ChooseAction::setFilter, this, &MyCanvas::setFilterDlg);
		connect(cos, &ChooseAction::generateTT, this, &MyCanvas::runGenerator);
		connect(cos, &ChooseAction::readData, this, &MyCanvas::readData);
		connect(cos, &ChooseAction::saveData, this, &MyCanvas::saveDataDlg);
		break;
	}
	case Qt::Key::Key_L:
	{
		AddLesson* cos = new AddLesson(nullptr, tableGen.getGroupsNames(), tableGen.getSubjectsNames(), tableGen.getTeachersNames());
		cos->show();
		connect(cos, &AddLesson::addLesson, this, &MyCanvas::getNewLesson);
		break;
	}
	case Qt::Key::Key_S:
	{
		AddSubject* cos = new AddSubject(nullptr);
		cos->show();
		connect(cos, &AddSubject::addSubject, this, &MyCanvas::getNewSubject);
		break;
	}
	case Qt::Key::Key_T:
	{
		AddTeacher* cos = new AddTeacher(nullptr);
		cos->show();
		connect(cos, &AddTeacher::addTeacher, this, &MyCanvas::getNewTeacher);
		break;
	}
	case Qt::Key::Key_G:
	{
		AddGroup* cos = new AddGroup(nullptr, tableGen.getGroupsNames());
		cos->show();
		connect(cos, &AddGroup::addGroup, this, &MyCanvas::getNewGroup);
		break;
	}
	case Qt::Key::Key_F:
	{
		SetFilter* cos = new SetFilter(nullptr, tableGen.getGroupsNames(), tableGen.getTeachersNames(), tableDrawer.getCurrentGroups());
		cos->show();
		connect(cos, &SetFilter::setFilter, this, &MyCanvas::setFilters);
		break;
	}
	case Qt::Key::Key_R:
	{
		tableGen.runGenerator();
		reloadFilter();
		updateAll();
		drawn = false;
		break;
	}
	case Qt::Key::Key_O:
	{
		std::ofstream plik("output.csv");
		tableGen.saveOutput(plik);
		plik.close();
		break;
	}

	}//end switch
}

void MyCanvas::getNewLesson(std::vector<std::string> data)
{
	int howManyTimes = std::stoi(data[3]);
	for (int i = 0; i < howManyTimes; i++)
		tableGen.addLesson(data[0], data[1], data[2]);
	reloadFilter();
	updateAll();
}

void MyCanvas::getNewSubject(std::vector<std::string> data)
{
	tableGen.addSubject(data[0], std::stoi(data[1]), std::stoi(data[2]));
}

void MyCanvas::getNewTeacher(std::vector<std::string> data)
{
	std::vector<ShortTime>present[WORK_WEEK];

	std::vector<std::string>::iterator i = data.begin();;
	std::string name = *i;
	i++;
	int h, m;
	for (int j = 0; j < WORK_WEEK; j++)
	{
		if (*i != "0") {
			h = std::stoi(*i);
			i++;
			m = std::stoi(*i);
			i++;
			present[j].push_back(ShortTime(h, m));

			h = std::stoi(*i);
			i++;
			m = std::stoi(*i);
			i++;
			present[j].push_back(ShortTime(h, m));
		}
		else
		{
			i++;
			i++;
			i++;
			i++;
		}
	}

	tableGen.addTeacher(name, present);
}

void MyCanvas::getNewGroup(std::vector<std::string> data)
{
	std::vector<Group*> parents;
	std::vector<std::string>::iterator i = data.begin();
	std::string groupName = *i;
	i++;

	for (; i != data.end(); i++)
	{
		parents.push_back(tableGen.getGroup(*i));
	}
	tableGen.addGroup(groupName, parents);
	reloadFilter();
	updateAll();
}

void MyCanvas::setFilters(std::vector<std::string> data)
{
	tableDrawer.clear();
	std::vector<std::string>::iterator i = data.begin();
	std::string teacherName = *i;
	if (teacherName != "Wszyscy")
		tableDrawer.setTeacherFilter(tableGen.getTeacher(teacherName)->getID());
	else
		tableDrawer.setTeacherFilter(-1);
	i++;

	for (; i != data.end(); i++)
	{
		tableDrawer.addGroupAndDescendants(tableGen.getGroup(*i));
	}
	drawn = false;
	reloadFilter();
	updateAll();
}

void MyCanvas::reloadFilter()
{
	std::vector<Group*> rel;
	for (auto i : tableDrawer.getCurrentGroupsIDs())
	{
		rel.push_back(tableGen.getGroup(i));
	}
	tableDrawer.clear();
	for (auto& i : rel)
	{
		tableDrawer.addGroupAndDescendants(i);
	}
}

void MyCanvas::resizeEvent(QResizeEvent * event) {
	QSFMLCanvas::resizeEvent(event);
	drawn = false;
	ResizeView(view);
}

void MyCanvas::wheelEvent(QWheelEvent * event) {
	drawn = false;
	if (event->delta() > 0)
	{
		view.zoom(0.95f);
	}
	else if (event->delta() < 0)
	{
		view.zoom(1.05f);
	}
	setView(view);
}

void MyCanvas::OnUpdate()
{
	if (!drawn)
	{
		setView(view);
		clear();
		tableDrawer.draw();
		drawn = true;
		display();
	}
}
