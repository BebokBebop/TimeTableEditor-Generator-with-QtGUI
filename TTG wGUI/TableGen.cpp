#include "TableGen.h"

int TableGen::shortTimeToInt(std::string text)
{
	int time;
	time = 0;
	time += text[0] - '0';
	time *= 10;
	time += text[1] - '0';
	time *= 10;
	//17 ':' 30
	time += text[3] - '0';
	time *= 10;
	time += text[4] - '0';
	return time;
}

void TableGen::readBreaks(std::ifstream & file)
{
	std::string testIfNum = "";
	file >> testIfNum;
	int num = 0;
	std::vector<int> times;
	while (testIfNum[0] >= '0' && testIfNum[0] <= '9')
	{
		times.push_back(shortTimeToInt(testIfNum));

		file >> testIfNum; // 15:00 '-' 15:30
		file >> testIfNum;

		times.push_back(shortTimeToInt(testIfNum));

		file >> testIfNum;
	}
	howManyBreaks = times.size() / 2;
	breaks = new ShortTime*[times.size() / 2];
	for (int i = 0, j = 0; j < times.size(); i++)
	{
		breaks[i] = new ShortTime[2];
		breaks[i][0] = times[j++];
		breaks[i][1] = times[j++];
	}


}

void TableGen::readTeachers(std::ifstream & file, int howManyRows) {
	std::vector<ShortTime> present[WORK_WEEK];
	std::string help;
	std::string name;
	int time;
	int ID;

	file >> help;
	while (true) {
		if (help != "Nazwy_lekcji_i_jak_dlugo:")
		{
			ID = stoi(help);
			file >> name;
			file >> help;
			name += " " + help;
		}
		else return;

		file >> help; //poniedzialek:
		file >> help;
		while (help != "wtorek:")
		{
			present[0].push_back(shortTimeToInt(help));
			file >> help; // 15:30 '-' 17:00
			file >> help;
			present[0].push_back(shortTimeToInt(help));
			file >> help;
		}
		file >> help;
		while (help != "sroda:")
		{
			present[1].push_back(shortTimeToInt(help));
			file >> help; // 15:30 '-' 17:00
			file >> help;
			present[1].push_back(shortTimeToInt(help));
			file >> help;
		}
		file >> help;
		while (help != "czwartek:")
		{
			present[2].push_back(shortTimeToInt(help));
			file >> help; // 15:30 '-' 17:00
			file >> help;
			present[2].push_back(shortTimeToInt(help));
			file >> help;
		}
		file >> help;
		while (help != "piatek:")
		{
			present[3].push_back(shortTimeToInt(help));
			file >> help; // 15:30 '-' 17:00
			file >> help;
			present[3].push_back(shortTimeToInt(help));
			file >> help;
		}
		file >> help;
		if (help.size() > 1)
		{
			while (help[2] == ':')
			{
				present[4].push_back(shortTimeToInt(help));
				file >> help; // 15:30 '-' 17:00
				file >> help;
				present[4].push_back(shortTimeToInt(help));
				file >> help;
				if (help.size() <= 1)
					break;
			}
		}
		teachers.push_back({ present, name, ID });

		if (help == "Nazwy_lekcji_i_jak_dlugo:")
			return;
	}
}

void TableGen::readSubjects(std::ifstream & file) {
	std::string help;
	std::string name;

	int ID;
	int timeInMin;
	file >> help;
	while (help != "Grupy:")
	{
		ID = stoi(help);
		file >> help; // '"'
		file >> name;
		file >> help;
		while (help != "\"")
		{
			name += " " + help;
			file >> help;
		}
		file >> timeInMin;
		subjects.push_back({ name, ID, timeInMin / 60, timeInMin % 60 });
		file >> help;
	}
}

void TableGen::readGroupsandLessons(std::ifstream & file, int howManyRows)
{
	int groupID;
	int teacherID;
	int howManyTimes;
	int lessonID = 0;
	int help2;

	std::string name = "";
	std::string help = "";
	std::vector<int> parentsID;

	if (file.eof())
	{
		return;
	}

	file >> help;
	while (!file.eof())
	{
		groupID = stoi(help);
		file >> name;
		groups.push_back({ name, groupID });

		file >> help;
		if (help == ":")
		{
			file >> help2;
			getGroup(help2)->addChild(getGroup(groupID));
			getGroup(groupID)->addParent(getGroup(help2));
			file >> help;
		}
		while (help != "{") {
			getGroup(std::stoi(help))->addChild(getGroup(groupID));
			getGroup(groupID)->addParent(getGroup(std::stoi(help)));
			file >> help;
		}
		file >> help;
		while (help != "}") {
			file >> teacherID;
			file >> howManyTimes;
			for (int i = 0; i < howManyTimes; i++) {
				lessons.push_back({ getGroup(groupID), getTeacher(teacherID),
					getSubject(stoi(help)), lessonID });
				getTeacher(teacherID)->addLesson(getLesson(lessonID));
				getGroup(groupID)->addLesson(getLesson(lessonID));
				lessonID++;
			}
			file >> help;
		}

		file >> help;
	}
}

void TableGen::readData(std::ifstream & file) {
	std::string dummy;
	file >> dummy;
	file >> start;

	file >> dummy;
	file >> end;

	file >> dummy;
	file >> timeUnit;

	file >> dummy;
	int howManyRows = (end - start) / timeUnit;
	readBreaks(file);

	readTeachers(file, howManyRows);

	readSubjects(file);

	readGroupsandLessons(file, howManyRows);


}

void TableGen::generateTimetableScaffolding() {
	ShortTime dayLength(end.getHour() - start.getHour(), end.getMinute() - start.getMinute());
	ShortTime breaksLength(0, 0);
	for (int i = 0; i < howManyBreaks; i++) {
		breaksLength.addTime(breaks[i][1].getHour() - breaks[i][0].getHour(), breaks[i][1].getMinute() - breaks[i][0].getMinute());
	}

	howManyRows = 0;
	howManyRows += howManyBreaks;
	bool wrap = false;
	while (!wrap)
	{
		if (dayLength.subTime(timeUnit)) {
			wrap = true;
			continue;
		}
		howManyRows++;
	}

	for (int i = 0; i < 1 + WORK_WEEK; i++) {//+1 for label
		timeTable[i] = new int[howManyRows];
		for (int j = 0; j < howManyRows; j++)
			timeTable[i][j] = 0;
	}

	ShortTime timeLabel1(start), timeLabel2(start + timeUnit);

	int breakID = 0;
	for (int i = 0; i < howManyRows; i++) {
		timeTable[0][i] = timeLabel1.getTime() * 10000 + timeLabel2.getTime();
		timeLabel1.addTime(timeUnit);
		timeLabel2.addTime(timeUnit);
		if (breakID + 1 < howManyBreaks)
			if (timeLabel2 > breaks[breakID][0]) {
				i++;
				if (i >= howManyRows) continue;
				timeTable[0][i] = breaks[breakID][0].getTime() * 10000 + breaks[breakID][1].getTime();
				breakID++;
				timeLabel1.setTime(breaks[breakID][1]);
				timeLabel2.setTime(timeLabel1 + timeUnit);
			}
	}
}

bool TableGen::generateTable()
{
	std::list<Lesson>::iterator less = lessons.begin();
	int id = 0;
	while (less != lessons.end())
	{
		less->updAvailability();
		less->cutOutMisfits();
		if (!(less->claimRandom())) //false - nie ma mozliwosci
		{
			if (less == lessons.begin())
				return false;
			else
			{
				less->unblock();
				less--;
				id--;
				if (id < 3) {
					std::cout << "resetting lesson #" << id << ' ';
					for (int i = 0; i < id; i++)
						std::cout << '#';
					std::cout << std::endl;
				}
				less->blockReclaim();
			}
		}
		else
		{
			less++;
			id++;
		}
	}
	return true;
}

void TableGen::sortLessons()
{
	std::list<Lesson>::iterator iter = lessons.begin();
	std::list<Lesson>::iterator iter2;
	Lesson* temp = nullptr;

	int max = 0;
	int tempRows;
	for (; iter != lessons.end(); iter++)
	{
		for (iter2 = iter; iter2 != lessons.end(); iter2++)
		{
			tempRows = iter2->getMyRows();
			if (tempRows > max) {
				max = tempRows;
				temp = &*iter2;
			}
		}
		max = 0;
		if (temp != nullptr)
			temp->swap(&*iter);
	}
}

bool TableGen::checkCollision(Lesson * A, Lesson * B)
{
	if (A->getMyDay() != B->getMyDay())
		return false;

	bool teacherConflict = false;
	bool groupConflict = false;
	if (A->getTeacher()->getID() == B->getTeacher()->getID())
		teacherConflict = true;
	if (A->getGroup()->getID() == B->getGroup()->getID()
		|| A->getGroup()->containsDescendantGroupID(B->getGroup()->getID())
		|| B->getGroup()->containsDescendantGroupID(A->getGroup()->getID()))
		groupConflict = true;

	if (teacherConflict || groupConflict)
	{
		ShortTime lenA(0);

		for (int i = 0; i < A->getMyRows(); i++)
			lenA.addTime(15);

		ShortTime lenB(0);

		for (int i = 0; i < B->getMyRows(); i++)
			lenB.addTime(15);

		ShortTime endA(A->getMyTime());
		endA.addTime(lenA);
		ShortTime endB(B->getMyTime());
		endB.addTime(lenB);

		if (A->getMyTime() >= endB || endA <= B->getMyTime());
		else
			return true;
	}
	return false;
}

void TableGen::unlockAllLessons()
{
	for (auto& i : lessons) {
		i.unblock();
		i.setDayAndRow(0, 0);
		i.setSet(false);
	}
}

void TableGen::unlockAllTeachers()
{
	for (auto& i : teachers) {
		i.castPresentToAvailability();
	}
}

void TableGen::unlockAllGroups()
{
	for (auto& i : groups) {
		i.setAvailability(true);
	}
}

TableGen::TableGen(ShortTime start, ShortTime end, ShortTime timeUnit, ShortTime * breakBegin, ShortTime * breakEnd, int howManyBreaks)
	:start(start), end(end), timeUnit(timeUnit), breakBegin(breakBegin), breakEnd(breakEnd), howManyBreaks(howManyBreaks)
{
	timeTable = new int*[1 + WORK_WEEK]; //+1 for label
	generateTimetableScaffolding();
}

TableGen::TableGen(std::ifstream & file)
{
	readData(file);
	timeTable = new int*[1 + WORK_WEEK];
	generateTimetableScaffolding();
}

TableGen::TableGen()
{
	timeTable = new int*[1 + WORK_WEEK];
}

void TableGen::clear()
{
	teachers.clear();
	groups.clear();
	subjects.clear();
	lessons.clear();
}

void TableGen::saveOutput(std::ofstream & plik)
{
	std::string days[5];
	days[0] = "poniedzialek";
	days[1] = "wtorek";
	days[2] = "sroda";
	days[3] = "czwartek";
	days[4] = "piatek";
	for (auto& i : groups)
	{
		plik << i.getName() << '\t';
		for (auto& j : i.getLessons())
		{
			if (j->getSet()) {
				plik << j->getSubjName() << '\t' << j->getTeacher()->getName() << '\t' << days[j->getMyDay()] << '\t' << j->getMyTime();
				plik << "\n\t";
			}
		}
		plik << '\n';
	}
}

void TableGen::presentLessons()
{
	std::string days[5];
	days[0] = "poniedzialek";
	days[1] = "wtorek";
	days[2] = "sroda";
	days[3] = "czwartek";
	days[4] = "piatek";
	for (auto& i : groups)
	{
		std::cout << i.getName() << '\t';
		for (auto& j : i.getLessons())
		{
			if (j->getSet()) {
				std::cout << j->getSubjName() << '\t' << j->getTeacher()->getName() << '\t' << days[j->getMyDay()] << '\t' << j->getMyTime();
				std::cout << "\n\t";
			}
		}
		std::cout << '\n';
	}
}

void TableGen::read(std::ifstream & file)
{
	clear();
	readData(file);
	generateTimetableScaffolding();
}

void TableGen::addSubject(std::string subjectName, int subjTimeHour, int subjTimeMinute)
{
	for (auto& i : subjects)
	{
		if (subjectName == i.getName())
			return;
	}
	subjects.push_back({ subjectName, int(subjects.size()), subjTimeHour,subjTimeMinute });
}

void TableGen::addTeacher(std::string teacherName, std::vector<ShortTime> present[WORK_WEEK])
{
	for (auto& i : teachers)
	{
		if (teacherName == i.getName())
			return;
	}
	teachers.push_back({ present, teacherName, (int)teachers.size() });
}

void TableGen::addLesson(std::string groupName, std::string subjectName, std::string teacherName)
{
	Group* myGroup = getGroup(groupName);
	Teacher* myTeacher = getTeacher(teacherName);

	lessons.push_back({ myGroup, myTeacher, getSubject(subjectName), (int)lessons.size() });
	Lesson* newLes = getLesson(lessons.size() - 1);
	myGroup->addLesson(newLes);
	myTeacher->addLesson(newLes);
}

void TableGen::addGroup(std::string groupName, std::vector<Group*> parents)
{
	for (auto& i : groups)
	{
		if (groupName == i.getName())
			return;
	}
	groups.push_back({ groupName, (int)groups.size(), parents });
	Group* newGroup = getGroup(groups.size() - 1);
	for (auto& i : parents)
	{
		i->addChild(newGroup);
	}
}

Group * TableGen::getGroup(int id)
{
	for (auto& i : groups)
		if (i.getID() == id)
			return &i;
	return nullptr;
}

Group * TableGen::getGroup(std::string name)
{
	for (auto& i : groups)
		if (i.getName() == name)
			return &i;
	return nullptr;
}

std::vector<std::string> TableGen::getGroupsNames()
{
	std::vector<std::string> ret;
	for (auto& i : groups)
		ret.push_back(i.getName());
	return ret;
}

Subject * TableGen::getSubject(int id)
{
	std::list<Subject>::iterator i;
	for (i = subjects.begin(); i != subjects.end(); i++)
		if (i->getID() == id)
			return &(*i);
	return nullptr;
}

Subject * TableGen::getSubject(std::string name)
{
	std::list<Subject>::iterator i;
	for (i = subjects.begin(); i != subjects.end(); i++)
		if (i->getName() == name)
			return &(*i);
	return nullptr;
}

std::vector<std::string> TableGen::getSubjectsNames()
{
	std::vector<std::string> ret;
	for (auto& i : subjects)
		ret.push_back(i.getName());
	return ret;
}

Teacher * TableGen::getTeacher(int id)
{
	std::list<Teacher>::iterator i;
	for (i = teachers.begin(); i != teachers.end(); i++)
		if (i->getID() == id)
			return &(*i);
	return nullptr;
}

Teacher * TableGen::getTeacher(std::string name)
{
	std::list<Teacher>::iterator i;
	for (i = teachers.begin(); i != teachers.end(); i++)
		if (i->getName() == name)
			return &(*i);
	return nullptr;
}

std::vector<std::string> TableGen::getTeachersNames()
{
	std::vector<std::string> ret;
	for (auto& i : teachers)
		ret.push_back(i.getName());
	return ret;
}

Lesson * TableGen::getLesson(int id)
{
	std::list<Lesson>::iterator i;
	for (i = lessons.begin(); i != lessons.end(); i++)
		if (i->getID() == id)
			return &(*i);
	return nullptr;
}

void TableGen::reset() {
	unlockAllLessons();
	unlockAllTeachers();
	unlockAllGroups();
}

bool TableGen::runGenerator()
{
	reset();
	sortLessons();
	if (!generateTable()) {
		reset();
		return false;
	}
	presentLessons();
	return true;
}

bool TableGen::updateCollisions(Lesson * les)
{
	bool ret = false;

	if (les->checkTeacherAvailabilityConflict())
	{
		les->setTeacherColl(false);
	}
	else
	{
		les->setTeacherColl(true);
		ret = true;
	}

	for (auto& i : lessons)
	{
		if (les->getID() == i.getID())
			continue;
		//if (i.getID() == 0 )
		//{
		//	i.getID();
		//}
		if (checkCollision(&i, les))
		{
			i.addCollidingLesson(les);
			les->addCollidingLesson(&i);
			ret = true;
		}
		else
		{
			i.removeCollidingLesson(les->getID());
			les->removeCollidingLesson(i.getID());
		}
	}
	return ret;
}

void TableGen::updateLesson(int groupID, int lessonID)
{
	for (auto& i : groups)
	{
		if (i.getID() == groupID)
			for (auto& j : i.getLessons())
			{
				if (j->getID() == lessonID) {
					updateCollisions(&*j);
					j->setSet(true);
				}
			}
	}
}
