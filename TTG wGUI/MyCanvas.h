#pragma once
#include "QSFMLCanvas.h"
#include <QtWidgets/qframe.h>
#include "TableDrawing.h"
#include "TableGen.h"
#include "windowDialogs.h"
#include <string>
#include <vector>


class MyCanvas : public QSFMLCanvas
{
public:
	MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size);

#define VIEW_HEIGHT 1000.f

private:
	sf::Vector2f previousMousePosition;
	sf::Vector2f mouseMoveDelta;
	sf::Vector2i pixelPos;
	bool moving = false;
	bool dragging = false;
	bool drawn = false;
	sf::View view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(VIEW_HEIGHT, VIEW_HEIGHT));
	TableDrawer tableDrawer = TableDrawer(*this);
	TableGen tableGen;

	void ResizeView(sf::View& view);

	void updateAll();

	void OnInit();

	virtual void closeEvent(QCloseEvent *close);

	virtual void mousePressEvent(QMouseEvent *event);

	virtual void mouseReleaseEvent(QMouseEvent *event);

	virtual void mouseMoveEvent(QMouseEvent *event);

	void addLessonDlg();
	void addSubjDlg();
	void addTeachDlg();
	void addGroupDlg();
	void setFilterDlg();
	void runGenerator();
	void saveDataDlg();
	void readData();

	virtual void keyPressEvent(QKeyEvent *event);

	void getNewLesson(std::vector<std::string> data);
	void getNewSubject(std::vector<std::string> data);
	void getNewTeacher(std::vector<std::string> data);
	void getNewGroup(std::vector<std::string> data);
	void setFilters(std::vector<std::string> data);
	void reloadFilter();
	virtual void resizeEvent(QResizeEvent *event);

	virtual void wheelEvent(QWheelEvent *event);

	void OnUpdate();
};