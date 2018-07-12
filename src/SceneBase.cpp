#include "SceneBase.h"

SceneBase::SceneBase(Physics* physics, QObject* parent) :
	QObject(parent),
	m_Physics(physics),
	m_Scene(NULL),
	m_DefaultMaterial(NULL)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Initialize()
{
	physx::PxSceneDesc sceneDescriptor(m_Physics->GetPhysics()->getTolerancesScale());
	sceneDescriptor.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDescriptor.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	sceneDescriptor.filterShader  = &physx::PxDefaultSimulationFilterShader;

	m_Scene = m_Physics->GetPhysics()->createScene(sceneDescriptor);
	m_Physics->SetActiveScene(m_Scene);

	m_DefaultMaterial = m_Physics->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);

	OnInitialize();
}

void SceneBase::Deinitialize()
{
	m_Physics->SetActiveScene(NULL);

	OnDeinitialize();

	m_Physics->CleanActors();
	m_Scene->release();
}

void SceneBase::OnInitialize()
{
}

void SceneBase::OnDeinitialize()
{
}
