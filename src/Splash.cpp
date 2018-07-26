#include "Splash.h"

Splash::Splash(QWidget *parent) :
	QSplashScreen(parent)
{
#ifdef Q_OS_SYMBIAN
	setAttribute(Qt::WA_LockLandscapeOrientation);
#else
	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(640, 360), qApp->desktop()->availableGeometry()));
#endif
	installEventFilter(this);
}

Splash::~Splash()
{
}

bool Splash::eventFilter(QObject *watched, QEvent *event)
{
	if((watched == this) && ((event->type() == QEvent::MouseButtonPress) || (event->type() == QEvent::MouseButtonRelease)))
		return true;
	else
		return QObject::eventFilter(watched, event);
}

void Splash::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);

	QPainter p(this);

	if(m_Logo.isNull() == true)
	{
		m_Logo = QPixmap(":/images/logo.png");
	}

	p.setPen(QColor(0, 0, 0, 255));
	p.setOpacity(1.0f);
	p.fillRect(QRect(0, 0, width(), height()), QColor(0, 0, 0, 255));

	p.drawPixmap(width() / 2 - m_Logo.width() / 2, height() / 2 - m_Logo.height() / 2, m_Logo);
}
