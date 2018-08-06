#include "Scene02.h"

float         Scene02::FUNNEL_VERTICES   [(Scene02::FUNNEL_VERTICES_COUNT + 1) * 8];
ushort        Scene02::FUNNEL_INDICES    [Scene02::FUNNEL_VERTICES_COUNT * 3];
physx::PxVec3 Scene02::FUNNEL_PX_VERTICES[Scene02::FUNNEL_VERTICES_COUNT + 1];
physx::PxU32  Scene02::FUNNEL_PX_INDICES [Scene02::FUNNEL_VERTICES_COUNT * 3];

QVector<float> Scene02::Funnel::GetVertices() const
{
	QVector<float> vertices = QVector<float>((Scene02::FUNNEL_VERTICES_COUNT + 1) * 8);
	memcpy(vertices.data(), Scene02::FUNNEL_VERTICES, sizeof(float) * vertices.size());
	return vertices;
}

QVector<ushort> Scene02::Funnel::GetIndices() const
{
	QVector<ushort> indices = QVector<ushort>(Scene02::FUNNEL_VERTICES_COUNT * 3);
	memcpy(indices.data(), Scene02::FUNNEL_INDICES, sizeof(ushort) * indices.size());
	return indices;
}

Scene02::Scene02(Physics* physics, QObject* parent) :
	SceneBase(physics, parent)
{
	const float radAngleStep = 1.0f / Scene02::FUNNEL_VERTICES_COUNT * 6.283185307179586476925286766559f;
	float radAngle           = 0.0f;

	for (uint idx = 0; idx < Scene02::FUNNEL_VERTICES_COUNT; idx++, radAngle += radAngleStep)
	{
		float angleSin = sinf(radAngle);
		float angleCos = cosf(radAngle);

		FUNNEL_PX_VERTICES[idx].x = angleCos * 100.0f;
		FUNNEL_PX_VERTICES[idx].y = 40.0f;
		FUNNEL_PX_VERTICES[idx].z = angleSin * 100.0f;

		FUNNEL_VERTICES[idx * 8 + 0] = angleCos * 100.0f;
		FUNNEL_VERTICES[idx * 8 + 1] = 40.0f;
		FUNNEL_VERTICES[idx * 8 + 2] = angleSin * 100.0f;

		FUNNEL_VERTICES[idx * 8 + 3] = -angleCos;
		FUNNEL_VERTICES[idx * 8 + 4] = 0.0f;
		FUNNEL_VERTICES[idx * 8 + 5] = -angleSin;

		FUNNEL_VERTICES[idx * 8 + 6] = angleCos;
		FUNNEL_VERTICES[idx * 8 + 7] = angleSin;

		FUNNEL_PX_INDICES[idx * 3 + 0] = (idx + 1) % Scene02::FUNNEL_VERTICES_COUNT;
		FUNNEL_PX_INDICES[idx * 3 + 1] = idx;
		FUNNEL_PX_INDICES[idx * 3 + 2] = Scene02::FUNNEL_VERTICES_COUNT;

		FUNNEL_INDICES[idx * 3 + 0] = FUNNEL_PX_INDICES[idx * 3 + 0];
		FUNNEL_INDICES[idx * 3 + 1] = FUNNEL_PX_INDICES[idx * 3 + 1];
		FUNNEL_INDICES[idx * 3 + 2] = FUNNEL_PX_INDICES[idx * 3 + 2];
	}

	physx::PxVec3* pxCenter = &FUNNEL_PX_VERTICES[Scene02::FUNNEL_VERTICES_COUNT];
	pxCenter->x = 0.0f;
	pxCenter->y = 0.0f;
	pxCenter->z = 0.0f;

	float* vxCenter = &FUNNEL_VERTICES[Scene02::FUNNEL_VERTICES_COUNT * 8];
	vxCenter[0] = 0.0f;
	vxCenter[1] = 0.0f;
	vxCenter[2] = 0.0f;
	vxCenter[3] = 0.0f;
	vxCenter[4] = 1.0f;
	vxCenter[5] = 0.0f;
	vxCenter[6] = 0.0f;
	vxCenter[7] = 0.0f;
}

Scene02::~Scene02()
{
}

void Scene02::OnInitialize()
{
	m_BoxMaterial = PhysicsEngine->GetPhysics()->createMaterial(0.5f, 0.001f, 1.0f);

	physx::PxTriangleMeshDesc meshDescriptor;
	meshDescriptor.points.count     = Scene02::FUNNEL_VERTICES_COUNT + 1;
	meshDescriptor.points.stride    = sizeof(physx::PxVec3);
	meshDescriptor.points.data      = FUNNEL_PX_VERTICES;
	meshDescriptor.triangles.count  = Scene02::FUNNEL_VERTICES_COUNT;
	meshDescriptor.triangles.stride = 3 * sizeof(physx::PxU32);
	meshDescriptor.triangles.data   = FUNNEL_PX_INDICES;

	physx::PxDefaultMemoryOutputStream outStream(*PhysicsEngine->GetAllocator());
	PhysicsEngine->GetCooking()->cookTriangleMesh(meshDescriptor, outStream);

	physx::PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
	physx::PxTriangleMesh* mesh = PhysicsEngine->GetPhysics()->createTriangleMesh(inStream);

	physx::PxRigidStatic* meshStatic = physx::PxCreateStatic(
		*PhysicsEngine->GetPhysics(),
		physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),
		physx::PxTriangleMeshGeometry(mesh),
		*DefaultMaterial);

	PhysicsEngine->AddMesh(new Funnel(meshStatic), QVector4D(0.2f, 0.3f, 0.4f, 16.0f), QVector2D(100.0f, 100.0f));

	for (uint idx = 0; idx < SPHERES_COUNT; idx++)
	{
		m_Objects.push_back(physx::PxCreateDynamic(
			*PhysicsEngine->GetPhysics(),
			physx::PxTransform(physx::PxVec3(
				(float)(rand() % 1000) * 0.04f - 20.0f,
				(float)(rand() % 1000) * 0.03f + 10.0f,
				(float)(rand() % 1000) * 0.04f - 20.0f)),
			physx::PxSphereGeometry(0.5f),
			*DefaultMaterial,
			10.0f));

		PhysicsEngine->AddSpere(m_Objects.back(), QVector4D(0.9f, 0.2f, 0.2f, 128.0f), QVector2D(1.0f, 1.0f));
	}

	for (uint idx = 0; idx < BOXES_COUNT; idx++)
	{
		m_Objects.push_back(physx::PxCreateDynamic(
			*PhysicsEngine->GetPhysics(),
			physx::PxTransform(physx::PxVec3(
				(float)(rand() % 1000) * 0.04f - 20.0f,
				(float)(rand() % 1000) * 0.03f + 10.0f,
				(float)(rand() % 1000) * 0.04f - 20.0f)),
			physx::PxBoxGeometry(0.5f, 0.5f, 0.5f),
			*m_BoxMaterial,
			0.1f));

		PhysicsEngine->AddBox(m_Objects.back(), QVector4D(1.0f, 0.8f, 0.6f, 16.0f), QVector2D(1.0f, 1.0f));
	}
}

void Scene02::OnDeinitialize()
{
	m_BoxMaterial->release();
	m_Objects.clear();
}

void Scene02::OnUpdate()
{
	foreach (sphere, m_Objects)
	{
		physx::PxRigidDynamic* sphereDynamic = *sphere;
		const physx::PxVec3 position = sphereDynamic->getGlobalPose().p;

		if (sqrtf(position.x * position.x + position.y * position.y + position.z * position.z) > 2.0f)
			continue;

		sphereDynamic->setLinearVelocity(sphereDynamic->getLinearVelocity() + physx::PxVec3(0.0f, 15.0f, 0.0f));
	}
}
