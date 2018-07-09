#include "Sample00.h"

Sample00::Sample00(QObject *parent) :
	SampleBase(parent),
	m_Material(NULL)
{
}

Sample00::~Sample00()
{
	m_Material->release();
}

void Sample00::Initialize()
{
	SampleBase::Initialize();

	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.5f);

	physx::PxRigidStatic* plane = physx::PxCreateStatic(
		*m_Physics,
		physx::PxTransform(physx::PxVec3(0.0f, -0.5f, 0.0f)),
		physx::PxBoxGeometry(100.5f, 0.5f, 100.5f),
		*m_Material);

	AddBox(plane, QVector3D(0.2f, 0.3f, 0.4f));

	for(uint idx = 0; idx < BOXES_COUNT; idx++)
	{
		physx::PxRigidDynamic* box = physx::PxCreateDynamic(
			*m_Physics,
			physx::PxTransform(0.0f, 2.5f + ((float)idx * 2.0f), 0.0f, physx::PxQuat((float)idx * 0.1f, physx::PxVec3(1.0f, 0.0f, 0.0f))),
			physx::PxBoxGeometry(0.5f, 0.5f, 0.5f),
			*m_Material,
			10.0f);

		AddBox(box, QVector3D(1.0f, 0.8f, 0.6f));
	}
}
