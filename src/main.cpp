#include <QApplication>
#include <QCoreApplication>
#include <QPixmap>

#include "Splash.h"
#include "MainWindow.h"
#include "GlWidget.h"
#include "Physics.h"
#include "Scene00.h"
#include "Scene01.h"
#include "Scene02.h"

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

#ifdef Q_OS_SYMBIAN
	QCoreApplication::setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);
#endif

	Splash* splash = new Splash();
#ifdef Q_OS_SYMBIAN
	splash->showFullScreen();
#else
	splash->show();
#endif

	application.processEvents();

	MainWindow mainWindow(splash, 2000);
	Physics    physics;
	Scene00    sample00(&physics);
	Scene01    sample01(&physics);
	Scene02    sample02(&physics);

	GlWidget *glWidget = mainWindow.GetGlWidget();
	QObject::connect(glWidget, SIGNAL(StatsUpdated_RenderSec(float)), &physics, SLOT(SetAdditionalDelta(float)));
	QObject::connect(&physics, SIGNAL(Simulated()), glWidget, SLOT(update()));
	QObject::connect(&physics, SIGNAL(ActorBoxAdded   (const ISceneObjectProvider*, QVector4D, QVector2D)), glWidget, SLOT(AddBox(const ISceneObjectProvider*,   QVector4D, QVector2D)));
	QObject::connect(&physics, SIGNAL(ActorSphereAdded(const ISceneObjectProvider*, QVector4D, QVector2D)), glWidget, SLOT(AddSpere(const ISceneObjectProvider*, QVector4D, QVector2D)));
	QObject::connect(&physics, SIGNAL(ActorMeshAdded  (const ISceneObjectProvider*, QVector4D, QVector2D)), glWidget, SLOT(AddMesh(const ISceneObjectProvider*,  QVector4D, QVector2D)));

	QObject::connect(glWidget, SIGNAL(Initialized()), &physics, SLOT(Initialize()));

	QObject::connect(glWidget, SIGNAL(StatsUpdated_SceneIdx(uint)), &mainWindow, SLOT(UpdateStats_SceneIdx(uint)));
	QObject::connect(glWidget, SIGNAL(StatsUpdated_RenderMs(float)), &mainWindow, SLOT(UpdateStats_RenderMs(float)));
	QObject::connect(&physics, SIGNAL(StatsUpdated_SimulateMs(float)), &mainWindow, SLOT(UpdateStats_SimulateMs(float)));
	QObject::connect(&physics, SIGNAL(StatsUpdated_AwakeActors(uint)), &mainWindow, SLOT(UpdateStats_AwakeActors(uint)));

	// delayed main window show in Initialize()

	glWidget->AddScene(&sample00);
	glWidget->AddScene(&sample01);
	glWidget->AddScene(&sample02);

	return application.exec();
}
