#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainApp.h"

class MainApp : public QMainWindow
{
	Q_OBJECT

public:
	MainApp(QWidget *parent = Q_NULLPTR);

private:
	Ui::MainAppClass ui;
};
