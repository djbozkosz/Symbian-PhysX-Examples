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
	const GlConstants::Mesh* mesh = GlConstants::GetDiamond(DIAMOND_VERTICES_COUNT);

	physx::PxConvexMeshDesc meshDescriptor;
	meshDescriptor.points.count  = mesh->PxVertices.size();
	meshDescriptor.points.stride = sizeof(physx::PxVec3);
	meshDescriptor.points.data   = mesh->PxVertices.constData();
	meshDescriptor.flags         = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream outStream(*PhysicsEngine->GetAllocator());
	PhysicsEngine->GetCooking()->cookConvexMesh(meshDescriptor, outStream);

	physx::PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
	physx::PxConvexMesh* convexMesh = PhysicsEngine->GetPhysics()->createConvexMesh(inStream);

	Objects.push_back(physx::PxCreateDynamic(
		*PhysicsEngine->GetPhysics(),
		physx::PxTransform(physx::PxVec3(
			5.0f, 20.0f, 5.0f)),
		physx::PxConvexMeshGeometry(convexMesh),
		*DefaultMaterial,
		10.0f));

	PhysicsEngine->AddMesh(new Diamond(Objects.back()), QVector4D(0.2f, 1.0f, 0.2f, 128.0f), QVector2D(1.0f, 1.0f));
}
