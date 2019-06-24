#include "MainApp.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qframe.h>
#include "MyCanvas.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MyCanvas* SFMLView = new MyCanvas(nullptr, QPoint(20, 20), QSize(1000, 800));
	SFMLView->show();

	return a.exec();
}
