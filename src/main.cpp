#include <QApplication>
#include <QCoreApplication>
#include <QPixmap>

#include "Splash.h"
#include "GlWidget.h"
#include "Sample00.h"

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	QCoreApplication::setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);

	Splash splash;
	splash.showFullScreen();

	application.processEvents();

	Sample00 sample00;
	GlWidget glWidget(NULL, &splash, 2000);

	QObject::connect(&sample00, SIGNAL(Simulated()), &glWidget, SLOT(update()));
	QObject::connect(&sample00, SIGNAL(ActorBoxAdded(const ISceneObjectProvider*, QVector3D, QVector2D)), &glWidget, SLOT(AddBox(const ISceneObjectProvider*, QVector3D, QVector2D)));
	QObject::connect(&sample00, SIGNAL(ActorMeshAdded(const ISceneObjectProvider*, QVector3D, QVector2D)), &glWidget, SLOT(AddMesh(const ISceneObjectProvider*, QVector3D, QVector2D)));

	QObject::connect(&glWidget, SIGNAL(Initialized()), &sample00, SLOT(Initialize()));

	// delayed glWidget show in Initialize()

	return application.exec();
}
