#pragma once
#include <Qt3DInput/qmouseevent.h>
#include <QtUiTools/quiloader.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>

class FailDlg : public QWidget
{
	Q_OBJECT
public:
	FailDlg(QWidget * parent) : QWidget(parent)
	{
		QWidget();
		QUiLoader loader;
		QFile file(":/MainApp/windowFailDlg.ui");
		file.open(QFile::ReadOnly);
		QWidget *myWidget = loader.load(&file, this);
		file.close();

		QPushButton* dodajButt = findChild<QPushButton*>("pushButton");
		connect(dodajButt, &QPushButton::clicked, this, &QWidget::close);


		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(myWidget);
		setLayout(layout);
	}
};


class ChooseAction : public QWidget
{
	Q_OBJECT
public:
	ChooseAction(QWidget * parent) : QWidget(parent)
	{
		QWidget();
		QUiLoader loader;
		QFile file(":/MainApp/windowChooseAction.ui");
		file.open(QFile::ReadOnly);
		QWidget *myWidget = loader.load(&file, this);
		file.close();

		QPushButton* addLesButt = findChild<QPushButton*>("pushButton_2");
		connect(addLesButt, &QPushButton::clicked, this, &ChooseAction::addLesson);

		QPushButton* addSubjBut = findChild<QPushButton*>("pushButton_8");
		connect(addSubjBut, &QPushButton::clicked, this, &ChooseAction::addSubj);

		QPushButton* addTeachButt = findChild<QPushButton*>("pushButton");
		connect(addTeachButt, &QPushButton::clicked, this, &ChooseAction::addTeach);

		QPushButton* addGroupButt = findChild<QPushButton*>("pushButton_3");
		connect(addGroupButt, &QPushButton::clicked, this, &ChooseAction::addGrp);

		QPushButton* setFilterButt = findChild<QPushButton*>("pushButton_4");
		connect(setFilterButt, &QPushButton::clicked, this, &ChooseAction::setFilter);

		QPushButton* generateTTButt = findChild<QPushButton*>("pushButton_5");
		connect(generateTTButt, &QPushButton::clicked, this, &ChooseAction::generateTT);

		QPushButton* readDataButt = findChild<QPushButton*>("pushButton_6");
		connect(readDataButt, &QPushButton::clicked, this, &ChooseAction::readData);

		QPushButton* saveDataButt = findChild<QPushButton*>("pushButton_7");
		connect(saveDataButt, &QPushButton::clicked, this, &ChooseAction::saveData);


		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(myWidget);
		setLayout(layout);
	}

signals:
	void addLesson();
signals:
	void addSubj();
signals:
	void addGrp();
signals:
	void addTeach();
signals:
	void setFilter();
signals:
	void generateTT();
signals:
	void readData();
signals:
	void saveData();

};

class AddLesson : public QWidget
{
	Q_OBJECT
public:
	AddLesson(QWidget * parent, std::vector<std::string> groups, std::vector<std::string> subjects, std::vector<std::string> teachers) : QWidget(parent)
	{
		QWidget();
		QUiLoader loader;
		QFile file(":/MainApp/windowAdd.ui");
		file.open(QFile::ReadOnly);
		QWidget *myWidget = loader.load(&file, this);
		file.close();

		QPushButton* dodajButt = findChild<QPushButton*>("pushButton");
		connect(dodajButt, &QPushButton::clicked, this, &AddLesson::preDodaj);
		QPushButton* anulujButt = findChild<QPushButton*>("pushButton_2");
		connect(anulujButt, &QPushButton::clicked, this, &QWidget::close);

		QComboBox* box1 = findChild<QComboBox*>("comboBox");
		QComboBox* box2 = findChild<QComboBox*>("comboBox_2");
		QComboBox* box3 = findChild<QComboBox*>("comboBox_3");

		for (auto& i : groups)
		{
			box1->addItem(QString::fromStdString(i));
		}
		for (auto& i : subjects)
		{
			box2->addItem(QString::fromStdString(i));
		}
		for (auto& i : teachers)
		{
			box3->addItem(QString::fromStdString(i));
		}


		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(myWidget);
		setLayout(layout);
	}
signals:
	void addLesson(std::vector<std::string> asd);

private slots:
	void preDodaj(bool co)
	{
		std::vector<std::string> ret;
		QComboBox* str1 = findChild<QComboBox*>("comboBox");
		QComboBox* str2 = findChild<QComboBox*>("comboBox_2");
		QComboBox* str3 = findChild<QComboBox*>("comboBox_3");
		QSpinBox* str4 = findChild<QSpinBox*>("spinBox");
		ret.push_back(str1->currentText().toStdString());
		ret.push_back(str2->currentText().toStdString());
		ret.push_back(str3->currentText().toStdString());
		ret.push_back(std::to_string(str4->value()));

		emit addLesson(ret);

		close();
	}
};

class AddSubject : public QWidget
{
	Q_OBJECT
public:
	AddSubject(QWidget * parent) : QWidget(parent)
	{
		QWidget();
		QUiLoader loader;
		QFile file(":/MainApp/windowAddSubject.ui");
		file.open(QFile::ReadOnly);
		QWidget *myWidget = loader.load(&file, this);
		file.close();

		QPushButton* dodajButt = findChild<QPushButton*>("pushButton");
		connect(dodajButt, &QPushButton::clicked, this, &AddSubject::preAddSubject);

		QPushButton* anulujButt = findChild<QPushButton*>("pushButton_2");
		connect(anulujButt, &QPushButton::clicked, this, &QWidget::close);

		QLineEdit* subjName = findChild<QLineEdit*>("lineEdit");
		subjName->insert(QString::fromStdString("<nazwa lekcji>"));

		QSpinBox* hours = findChild<QSpinBox*>("spinBox");
		QSpinBox* minutes = findChild<QSpinBox*>("spinBox_2");
		minutes->setSingleStep(15);
		minutes->setMaximum(45);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(myWidget);
		setLayout(layout);
	}
signals:
	void addSubject(std::vector<std::string> asd);

private slots:
	void preAddSubject(bool co)
	{
		std::vector<std::string> ret;
		QLineEdit* str2 = findChild<QLineEdit*>("lineEdit");
		QSpinBox* str3 = findChild<QSpinBox*>("spinBox");
		QSpinBox* str4 = findChild<QSpinBox*>("spinBox_2");
		ret.push_back(str2->text().toStdString());
		ret.push_back(std::to_string(str3->value()));
		ret.push_back(std::to_string(str4->value()));

		emit addSubject(ret);

		close();
	}
};

class AddTeacher : public QWidget
{
	Q_OBJECT
public:
	AddTeacher(QWidget * parent) : QWidget(parent)
	{
		QWidget();
		QUiLoader loader;
		QFile file(":/MainApp/windowAddTeacher.ui");
		file.open(QFile::ReadOnly);
		QWidget *myWidget = loader.load(&file, this);
		file.close();

		QPushButton* dodajButt = findChild<QPushButton*>("pushButton");
		connect(dodajButt, &QPushButton::clicked, this, &AddTeacher::preAddTeacher);
		QPushButton* anulujButt = findChild<QPushButton*>("pushButton_2");
		connect(anulujButt, &QPushButton::clicked, this, &QWidget::close);


		for (int spinBoxID = 1; spinBoxID <= 20; spinBoxID++)
		{
			{
				std::string spinBoxName = "spinBox_";
				spinBoxName += std::to_string(spinBoxID);
				QSpinBox* spBox = findChild<QSpinBox*>(QString::fromStdString(spinBoxName));

				spBox->setMaximum(24);
			}
			spinBoxID++;
			{
				std::string spinBoxName = "spinBox_";
				spinBoxName += std::to_string(spinBoxID);
				QSpinBox* spBox = findChild<QSpinBox*>(QString::fromStdString(spinBoxName));

				spBox->setMaximum(45);
				spBox->setSingleStep(15);
			}
		}

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(myWidget);
		setLayout(layout);
	}
signals:
	void addTeacher(std::vector<std::string> asd);

private slots:
	void preAddTeacher(bool co)
	{
		std::vector<std::string> ret;
		QLineEdit* str1 = findChild<QLineEdit*>("lineEdit");
		ret.push_back(str1->text().toStdString());

		int spinBoxID = 1;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::string spinBoxName = "spinBox_";
				spinBoxName += std::to_string(spinBoxID);
				QSpinBox* str4 = findChild<QSpinBox*>(QString::fromStdString(spinBoxName));
				ret.push_back(std::to_string(str4->value()));
				spinBoxID++;
			}
		}


		emit addTeacher(ret);

		close();
	}
};

class AddGroup : public QWidget
{
	Q_OBJECT
		std::vector<QCheckBox*> checkBoxVec;
public:
	AddGroup(QWidget * parent, std::vector<std::string> groups) : QWidget(parent)
	{
		QWidget();
		QUiLoader loader;
		QFile file(":/MainApp/windowAddGroup.ui");
		file.open(QFile::ReadOnly);
		QWidget *myWidget = loader.load(&file, this);
		file.close();

		QPushButton* dodajButt = findChild<QPushButton*>("pushButton");
		connect(dodajButt, &QPushButton::clicked, this, &AddGroup::preAddGroup);
		QPushButton* anulujButt = findChild<QPushButton*>("pushButton_2");
		connect(anulujButt, &QPushButton::clicked, this, &QWidget::close);

		QWidget* content = findChild<QWidget*>("scrollAreaWidgetContents");
		for (auto& i : groups)
		{
			QCheckBox* checkBox = new QCheckBox();
			checkBox->setText(QString::fromStdString(i));
			checkBoxVec.push_back(checkBox);
			content->layout()->addWidget(checkBox);
		}

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(myWidget);
		setLayout(layout);
	}
	~AddGroup()
	{
		QWidget::~QWidget();
		for (auto& i : checkBoxVec)
		{
			delete i;
		}
	}

signals:
	void addGroup(std::vector<std::string> asd);

private slots:
	void preAddGroup(bool co)
	{
		std::vector<std::string> ret;
		QLineEdit* str1 = findChild<QLineEdit*>("lineEdit");
		ret.push_back(str1->text().toStdString());

		for (auto& i : checkBoxVec)
		{
			if (i->isChecked())
				ret.push_back(i->text().toStdString());
		}
		emit addGroup(ret);

		close();
	}
};

class SetFilter : public QWidget
{
	Q_OBJECT
		std::vector<QCheckBox*> checkBoxVec;
public:
	SetFilter(QWidget * parent, std::vector<std::string> groups, std::vector<std::string> teachers, std::vector<std::string> currentFilter) : QWidget(parent)
	{
		QWidget();
		QUiLoader loader;
		QFile file(":/MainApp/windowSetFilter.ui");
		file.open(QFile::ReadOnly);
		QWidget *myWidget = loader.load(&file, this);
		file.close();

		QPushButton* dodajButt = findChild<QPushButton*>("pushButton");
		connect(dodajButt, &QPushButton::clicked, this, &SetFilter::presetFilter);
		QPushButton* anulujButt = findChild<QPushButton*>("pushButton_2");
		connect(anulujButt, &QPushButton::clicked, this, &QWidget::close);

		QWidget* content = findChild<QWidget*>("scrollAreaWidgetContents");
		for (auto& i : groups)
		{
			QCheckBox* checkBox = new QCheckBox();
			checkBox->setText(QString::fromStdString(i));
			checkBoxVec.push_back(checkBox);
			content->layout()->addWidget(checkBox);
			for (auto& j : currentFilter)
			{
				if (j == i)
				{
					checkBox->setChecked(true);
				}
			}
		}

		QComboBox* box = findChild<QComboBox*>("comboBox");

		box->addItem("Wszyscy");
		for (auto& i : teachers)
		{
			box->addItem(QString::fromStdString(i));
		}

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(myWidget);
		setLayout(layout);
	}
	~SetFilter()
	{
		QWidget::~QWidget();
		for (auto& i : checkBoxVec)
		{
			delete i;
		}
	}

signals:
	void setFilter(std::vector<std::string> asd);

private slots:
	void presetFilter(bool co)
	{
		std::vector<std::string> ret;
		QComboBox* box = findChild<QComboBox*>("comboBox");
		ret.push_back(box->currentText().toStdString());

		for (auto& i : checkBoxVec)
		{
			if (i->isChecked())
				ret.push_back(i->text().toStdString());
		}
		emit setFilter(ret);

		close();
	}
};
