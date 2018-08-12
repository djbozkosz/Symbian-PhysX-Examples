#include "Scene02.h"

Scene02::Scene02(Physics* physics, QObject* parent) :
	SceneBase(physics, parent)
{
}

Scene02::~Scene02()
{
}

void Scene02::OnInitialize()
{
	BoxMaterial = PhysicsEngine->GetPhysics()->createMaterial(0.5f, 0.001f, 1.0f);

	const GlConstants::Mesh* funnel = GlConstants::GetFunnel(FUNNEL_VERTICES_COUNT);

	physx::PxTriangleMesh* mesh = PhysicsEngine->CreateMesh(funnel->PxVertices, funnel->PxIndices);

	physx::PxRigidStatic* meshStatic = physx::PxCreateStatic(
		*PhysicsEngine->GetPhysics(),
		physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),
		physx::PxTriangleMeshGeometry(mesh),
		*DefaultMaterial);

	PhysicsEngine->AddMesh(new Funnel(meshStatic), QVector4D(0.2f, 0.3f, 0.4f, 16.0f), QVector2D(100.0f, 100.0f));

	OnInitializeObjects();
}

void Scene02::OnDeinitialize()
{
	BoxMaterial->release();
	Objects.clear();
}

void Scene02::OnUpdate()
{
	foreach (sphere, Objects)
	{
		physx::PxRigidDynamic* sphereDynamic = *sphere;
		const physx::PxVec3 position = sphereDynamic->getGlobalPose().p;

		if (sqrtf(position.x * position.x + position.y * position.y + position.z * position.z) > 2.0f)
			continue;

		sphereDynamic->setLinearVelocity(sphereDynamic->getLinearVelocity() + physx::PxVec3(0.0f, 15.0f, 0.0f));
	}
}

void Scene02::OnInitializeObjects()
{
	for (uint idx = 0; idx < SPHERES_COUNT; idx++)
	{
		Objects.push_back(physx::PxCreateDynamic(
			*PhysicsEngine->GetPhysics(),
			physx::PxTransform(physx::PxVec3(
				(float)(rand() % 1000) * 0.04f - 20.0f,
				(float)(rand() % 1000) * 0.03f + 10.0f,
				(float)(rand() % 1000) * 0.04f - 20.0f)),
			physx::PxSphereGeometry(0.5f),
			*DefaultMaterial,
			10.0f));

		PhysicsEngine->AddSpere(Objects.back(), QVector4D(0.9f, 0.2f, 0.2f, 128.0f), QVector2D(1.0f, 1.0f));
	}

	for (uint idx = 0; idx < BOXES_COUNT; idx++)
	{
		Objects.push_back(physx::PxCreateDynamic(
			*PhysicsEngine->GetPhysics(),
			physx::PxTransform(physx::PxVec3(
				(float)(rand() % 1000) * 0.04f - 20.0f,
				(float)(rand() % 1000) * 0.03f + 10.0f,
				(float)(rand() % 1000) * 0.04f - 20.0f)),
			physx::PxBoxGeometry(0.5f, 0.5f, 0.5f),
			*BoxMaterial,
			0.1f));

		PhysicsEngine->AddBox(Objects.back(), QVector4D(1.0f, 0.8f, 0.6f, 16.0f), QVector2D(1.0f, 1.0f));
	}
}
