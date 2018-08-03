#include "Scene02.h"

const physx::PxVec3 Scene02::VERTICES[] =
{
	physx::PxVec3(-50.0f, 20.0f, -50.0f),
	physx::PxVec3( 50.0f, 20.0f, -50.0f),
	physx::PxVec3(-50.0f, 20.0f,  50.0f),
	physx::PxVec3( 50.0f, 20.0f,  50.0f),
	physx::PxVec3(  0.0f,  0.0f,   0.0f)
};

const physx::PxU32 Scene02::INDICES[] =
{
	0, 2, 4,
	2, 3, 4,
	3, 1, 4,
	1, 0, 4
};

Scene02::Scene02(Physics* physics, QObject* parent) :
	SceneBase(physics, parent)
{
}

Scene02::~Scene02()
{
}

void Scene02::OnInitialize()
{
	physx::PxRigidStatic* plane = physx::PxCreateStatic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(0.0f, -0.5f, 0.0f)),
		physx::PxBoxGeometry(50.0f, 0.5f, 50.0f),
		*m_DefaultMaterial);

	m_Physics->AddBox(plane, QVector4D(0.2f, 0.3f, 0.4f, 16.0f), QVector2D(100.0f, 100.0f));

	physx::PxTriangleMeshDesc meshDescriptor;
	meshDescriptor.points.count     = 5;
	meshDescriptor.points.stride    = sizeof(physx::PxVec3);
	meshDescriptor.points.data      = VERTICES;
	meshDescriptor.triangles.count  = 4;
	meshDescriptor.triangles.stride = 3 * sizeof(physx::PxU32);
	meshDescriptor.triangles.data   = INDICES;

	physx::PxDefaultMemoryOutputStream outStream(*m_Physics->GetAllocator());
	m_Physics->GetCooking()->cookTriangleMesh(meshDescriptor, outStream);

	physx::PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
	physx::PxTriangleMesh* mesh = m_Physics->GetPhysics()->createTriangleMesh(inStream);

	physx::PxRigidStatic* meshStatic = physx::PxCreateStatic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),
		physx::PxTriangleMeshGeometry(mesh),
		*m_DefaultMaterial);

	m_Physics->AddMesh(meshStatic, QVector4D(0.2f, 0.3f, 0.4f, 16.0f), QVector2D(100.0f, 100.0f));

	for(uint idx = 0; idx < SPHERES_COUNT; idx++)
	{
		m_Spheres.push_back(physx::PxCreateDynamic(
			*m_Physics->GetPhysics(),
			physx::PxTransform(physx::PxVec3(
				(float)(rand() % 1000) * 0.04f - 20.0f,
				(float)(rand() % 1000) * 0.03f + 10.0f,
				(float)(rand() % 1000) * 0.04f - 20.0f)),
			physx::PxSphereGeometry(0.5f),
			*m_DefaultMaterial,
			10.0f));

		m_Physics->AddSpere(m_Spheres.back(), QVector4D(0.9f, 0.2f, 0.2f, 128.0f), QVector2D(1.0f, 1.0f));
	}

	for(uint idx = 0; idx < SPHERES_COUNT / 5; idx++)
	{
		m_Spheres.push_back(physx::PxCreateDynamic(
			*m_Physics->GetPhysics(),
			physx::PxTransform(physx::PxVec3(
				(float)(rand() % 1000) * 0.04f - 20.0f,
				(float)(rand() % 1000) * 0.03f + 10.0f,
				(float)(rand() % 1000) * 0.04f - 20.0f)),
			physx::PxBoxGeometry(0.5f, 0.5f, 0.5f),
			*m_DefaultMaterial,
			10.0f));

		m_Physics->AddBox(m_Spheres.back(), QVector4D(1.0f, 0.8f, 0.6f, 16.0f), QVector2D(1.0f, 1.0f));
	}
}

void Scene02::OnDeinitialize()
{
	m_Spheres.clear();
}

void Scene02::OnUpdate()
{
	foreach(sphere, m_Spheres)
	{
		physx::PxRigidDynamic* sphereDynamic = *sphere;
		const physx::PxVec3 position = sphereDynamic->getGlobalPose().p;

		if(sqrtf(position.x * position.x + position.y * position.y + position.z * position.z) > 2.0f)
			continue;

		sphereDynamic->setLinearVelocity(sphereDynamic->getLinearVelocity() + physx::PxVec3(0.0f, 15.0f, 0.0f));
	}
}
