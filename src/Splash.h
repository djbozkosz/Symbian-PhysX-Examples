#ifndef SPLASH_H
#define SPLASH_H

#include <QSplashScreen>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QRect>

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
