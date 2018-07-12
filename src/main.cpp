#include <QApplication>
#include <QCoreApplication>
#include <QPixmap>

#include "Splash.h"
#include "GlWidget.h"
#include "Physics.h"
#include "Scene00.h"
#include "Scene01.h"

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	QCoreApplication::setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);

	Splash splash;
	splash.showFullScreen();

	application.processEvents();

	GlWidget glWidget(&splash, 2000);
	Physics  physics;
	Scene00  sample00(&physics);
	Scene01  sample01(&physics);

	QObject::connect(&physics, SIGNAL(Simulated()), &glWidget, SLOT(update()));
	QObject::connect(&physics, SIGNAL(ActorBoxAdded(const ISceneObjectProvider*, QVector3D, QVector2D)), &glWidget, SLOT(AddBox(const ISceneObjectProvider*, QVector3D, QVector2D)));
	QObject::connect(&physics, SIGNAL(ActorSphereAdded(const ISceneObjectProvider*, QVector3D, QVector2D)), &glWidget, SLOT(AddSpere(const ISceneObjectProvider*, QVector3D, QVector2D)));
	QObject::connect(&physics, SIGNAL(ActorMeshAdded(const ISceneObjectProvider*, QVector3D, QVector2D)), &glWidget, SLOT(AddMesh(const ISceneObjectProvider*, QVector3D, QVector2D)));

	QObject::connect(&glWidget, SIGNAL(Initialized()), &physics, SLOT(Initialize()));

	// delayed glWidget show in Initialize()

	glWidget.AddScene(&sample00);
	glWidget.AddScene(&sample01);

	return application.exec();
}
