#include "MainWindow.h"

MainWindow::MainWindow(QSplashScreen* splash, uint splashDelayMs, QWidget* parent) :
	QMainWindow(parent),
	m_Ui(new Ui::MainWindow),
	m_SceneIdx(0),
	m_RenderMs(0.0f),
	m_SimulateMs(0.0f),
	m_AwakeActors(0)
{
	m_Ui->setupUi(this);

#ifdef Q_OS_SYMBIAN
	setAttribute(Qt::WA_LockLandscapeOrientation);
#endif

	connect(m_Ui->prevScene, SIGNAL(clicked()), m_Ui->glWidget, SLOT(ActivatePrevScene()));
	connect(m_Ui->nextScene, SIGNAL(clicked()), m_Ui->glWidget, SLOT(ActivateNextScene()));
	connect(m_Ui->close,     SIGNAL(clicked()), qApp,          SLOT(quit()));

	QTimer::singleShot(splashDelayMs, this, SLOT(Initialize()));
}

MainWindow::~MainWindow()
{
	delete m_Ui;
}

void MainWindow::Initialize()
{
	showFullScreen();
}

void MainWindow::UpdateStats()
{
	m_Ui->stats->setText
	(
		QString("Scene: %1, Render: %2 ms, Physics: %3 ms, Awake: %4")
		.arg(m_SceneIdx)
		.arg(m_RenderMs)
		.arg(m_SimulateMs)
		.arg(m_AwakeActors)
	);
}
