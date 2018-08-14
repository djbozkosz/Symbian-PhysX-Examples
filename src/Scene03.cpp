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

	Objects.push_back(physx::PxCreateDynamic(
		*PhysicsEngine->GetPhysics(),
		physx::PxTransform(physx::PxVec3(
			5.0f, 20.0f, 5.0f)),
		physx::PxConvexMeshGeometry(mesh),
		*DefaultMaterial,
		10.0f));

	PhysicsEngine->AddMesh(new Diamond(Objects.back()), QVector4D(0.2f, 1.0f, 0.2f, 128.0f), QVector2D(1.0f, 1.0f));
}
