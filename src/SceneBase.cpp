#include "SceneBase.h"

SceneBase::SceneBase(Physics* physics, QObject* parent) :
	QObject(parent),
	PhysicsEngine(physics),
	Scene(NULL),
	DefaultMaterial(NULL)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Initialize()
{
	physx::PxSceneDesc sceneDescriptor(PhysicsEngine->GetPhysics()->getTolerancesScale());
	sceneDescriptor.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDescriptor.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(0);
	sceneDescriptor.filterShader  = &physx::PxDefaultSimulationFilterShader;

	Scene = PhysicsEngine->GetPhysics()->createScene(sceneDescriptor);
	PhysicsEngine->SetActiveScene(this);

	DefaultMaterial = PhysicsEngine->GetPhysics()->createMaterial(0.5f, 0.5f, 0.5f);

	OnInitialize();
}

void SceneBase::Deinitialize()
{
	PhysicsEngine->SetActiveScene(NULL);

	OnDeinitialize();

	DefaultMaterial->release();

	PhysicsEngine->CleanActors();
	Scene->release();
}

void SceneBase::Update()
{
	OnUpdate();
}

physx::PxScene* SceneBase::GetScene() const
{
	return Scene;
}

void SceneBase::OnInitialize()
{
}

void SceneBase::OnDeinitialize()
{
}

void SceneBase::OnUpdate()
{
}
