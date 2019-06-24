#pragma once
#include <SFML/Graphics.hpp>
#include <QtWidgets/qwidget.h>
#include <QtCore/qtimer.h>

class QSFMLCanvas :
	public QWidget,
	public sf::RenderWindow
{
public:

	QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 30);

	virtual ~QSFMLCanvas() {}

protected:

	virtual void OnInit() {}

	virtual void OnUpdate() {}

	virtual QPaintEngine* paintEngine() const;

	virtual void showEvent(QShowEvent*);

	virtual void paintEvent(QPaintEvent*);

	virtual void resizeEvent(QResizeEvent* event);

	QTimer myTimer;
	bool   myInitialized;
};

