#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>

#include "GlWidget.h"
#include "Sample00.h"

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	QSplashScreen splash(QPixmap(":/images/logo.png"));
	splash.show();
	splash.setStyleSheet("background-color: black;");
	application.processEvents();

	Sample00 sample00;
	GlWidget glWidget;

	QObject::connect(&sample00, SIGNAL(Simulated()), &glWidget, SLOT(update()));
	QObject::connect(&sample00, SIGNAL(ActorBoxAdded(const ISceneObjectProvider*, QVector3D)), &glWidget, SLOT(AddBox(const ISceneObjectProvider*, QVector3D)));
	QObject::connect(&sample00, SIGNAL(ActorMeshAdded(const ISceneObjectProvider*, QVector3D)), &glWidget, SLOT(AddMesh(const ISceneObjectProvider*, QVector3D)));

	QObject::connect(&glWidget, SIGNAL(Initialized()), &sample00, SLOT(Initialize()));

	glWidget.showMaximized();
	splash.finish(&glWidget);

	return application.exec();
}
