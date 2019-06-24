#include "QSFMLCanvas.h"


QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime) :
	QWidget(Parent),
	myInitialized(false)
{
	// Setup some states to allow direct rendering into the widget
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);

	// Set strong focus to enable keyboard events to be received
	setFocusPolicy(Qt::StrongFocus);

	// Setup the widget geometry
	move(Position);
	resize(Size);

	// Setup the timer
	myTimer.setInterval(FrameTime);
}

void QSFMLCanvas::showEvent(QShowEvent*)
{
	if (!myInitialized)
	{
		// Create the SFML window with the widget handle
		sf::RenderWindow::create((sf::WindowHandle)winId());

		// Let the derived class do its specific stuff
		OnInit();

		// Setup the timer to trigger a refresh at specified framerate
		connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
		myTimer.start();

		myInitialized = true;
	}
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
	return 0;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
	OnUpdate();
}

void QSFMLCanvas::resizeEvent(QResizeEvent* event)
{
	setSize(sf::Vector2u(QWidget::width(), QWidget::height()));
}