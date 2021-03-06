#include "MainWindow.h"

MainWindow::MainWindow(QSplashScreen* splash, uint splashDelayMs, QWidget* parent) :
	QMainWindow(parent),
	m_Ui(new Ui::MainWindow),
	m_Splash(splash),
	m_SceneIdx(0),
	m_RenderMs(0.0f),
	m_SimulateMs(0.0f),
	m_AwakeActors(0)
{
	m_Ui->setupUi(this);

#ifdef Q_OS_SYMBIAN
	setAttribute(Qt::WA_LockLandscapeOrientation);
#else
	setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(640, 360), qApp->desktop()->availableGeometry()));
#endif

	m_Ui->prevScene->setMaximumWidth(m_Ui->prevScene->height());
	m_Ui->nextScene->setMaximumWidth(m_Ui->nextScene->height());
	m_Ui->close->    setMaximumWidth(m_Ui->close->    height());

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
#ifdef Q_OS_SYMBIAN
	showFullScreen();
#else
	show();
#endif

	m_Splash->deleteLater();
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
