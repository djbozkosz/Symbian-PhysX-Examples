#include "Scene03.h"

Scene03::Scene03(Physics* physics, QObject* parent) :
	Scene02(physics, parent)
{
}

Scene03::~Scene03()
{
}

void Scene03::OnInitializeObjects()
{
	const GlConstants::Mesh* diamond = GlConstants::GetDiamond(DIAMOND_VERTICES_COUNT);

	physx::PxConvexMesh* mesh = PhysicsEngine->CreateConvexMesh(diamond->Vertices, diamond->Indices);

	for (uint idx = 0; idx < DIAMONDS_COUNT; idx++)
	{
		Objects.push_back(physx::PxCreateDynamic(
			*PhysicsEngine->GetPhysics(),
			physx::PxTransform(physx::PxVec3(
				(float)(rand() % 1000) * 0.04f - 20.0f,
				(float)(rand() % 1000) * 0.03f + 10.0f,
				(float)(rand() % 1000) * 0.04f - 20.0f)),
			physx::PxConvexMeshGeometry(mesh),
			*BoxMaterial,
			10.0f));

		PhysicsEngine->AddMesh(new Diamond(Objects.back()), QVector4D(0.2f, 1.0f, 0.2f, 128.0f), QVector2D(1.0f, 1.0f));
	}
}
