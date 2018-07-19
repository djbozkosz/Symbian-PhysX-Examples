#include "Scene02.h"

const physx::PxVec3 Scene02::VERTICES[] =
{
	physx::PxVec3(-50.0f,   0.0f, -50.0f),
	physx::PxVec3( 50.0f,   0.0f, -50.0f),
	physx::PxVec3(-50.0f,   0.0f,  50.0f),
	physx::PxVec3( 50.0f,   0.0f,  50.0f),
	physx::PxVec3(  0.0f, -10.0f,   0.0f)
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
	physx::PxTriangleMeshDesc meshDescriptor;
	meshDescriptor.points.count     = 5;
	meshDescriptor.points.stride    = sizeof(physx::PxVec3);
	meshDescriptor.points.data      = VERTICES;
	meshDescriptor.triangles.count  = 4;
	meshDescriptor.triangles.stride = 3 * sizeof(physx::PxU32);
	meshDescriptor.triangles.data   = INDICES;

	physx::PxDefaultMemoryOutputStream outStream;
	if(m_Physics->GetCooking()->cookTriangleMesh(meshDescriptor, outStream) == false)
	{
		qDebug() << "Cannot cook mesh!";
	}

	physx::PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
	physx::PxTriangleMesh* mesh = m_Physics->GetPhysics()->createTriangleMesh(inStream);

	physx::PxRigidStatic* meshStatic = physx::PxCreateStatic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),
		physx::PxTriangleMeshGeometry(mesh),
		*m_DefaultMaterial);

	m_Physics->AddMesh(meshStatic, QVector4D(0.2f, 0.3f, 0.4f, 16.0f), QVector2D(100.0f, 100.0f));

	physx::PxRigidDynamic* sphere = physx::PxCreateDynamic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(5.0f, 10.0f, 5.0f)),
		physx::PxSphereGeometry(1.5f),
		*m_DefaultMaterial,
		10.0f);

	m_Physics->AddSpere(sphere, QVector4D(0.9f, 0.2f, 0.2f, 128.0f), QVector2D(1.0f, 1.0f));
}

Scene02::~Scene02()
{
}

void Scene02::OnInitialize()
{
}
