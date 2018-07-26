#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QSize>

#include <QTimer>

#include "ui_mainWindow.h"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	private:

	Q_OBJECT

	Ui::MainWindow* m_Ui;

	QSplashScreen* m_Splash;

	uint  m_SceneIdx;
	float m_RenderMs;
	float m_SimulateMs;
	uint  m_AwakeActors;

	public:

	explicit MainWindow(QSplashScreen* splash, uint splashDelayMs, QWidget* parent = NULL);
	~MainWindow();

	inline GlWidget* GetGlWidget() const { return m_Ui->glWidget; }

	public slots:

	inline void UpdateStats_SceneIdx(uint sceneIdx)       { m_SceneIdx    = sceneIdx;    UpdateStats(); }
	inline void UpdateStats_RenderMs(float renderMs)      { m_RenderMs    = renderMs;    UpdateStats(); }
	inline void UpdateStats_SimulateMs(float simulateMs)  { m_SimulateMs  = simulateMs;  UpdateStats(); }
	inline void UpdateStats_AwakeActors(uint awakeActors) { m_AwakeActors = awakeActors; UpdateStats(); }

	private slots:

	void Initialize();

	void UpdateStats();
};

#endif // MAINWINDOW_H
