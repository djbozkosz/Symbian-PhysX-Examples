#ifndef SPLASH_H
#define SPLASH_H

#include <QApplication>
#include <QSplashScreen>
#include <QDesktopWidget>
#include <QStyle>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QRect>
#include <QSize>

#include <QEvent>

class Splash : public QSplashScreen
{
	private:

	Q_OBJECT

	QPixmap m_Logo;

	public:

	explicit Splash(QWidget *parent = NULL);
	virtual ~Splash();

	virtual bool eventFilter(QObject *watched, QEvent *event);

	protected:
		virtual void paintEvent(QPaintEvent *e);
};

#endif // SPLASH_H
