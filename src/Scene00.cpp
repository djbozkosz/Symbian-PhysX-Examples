#include "Scene00.h"

Scene00::Scene00(Physics* physics, QObject* parent) :
	SceneBase(physics, parent)
{
}

Scene00::~Scene00()
{
}

void Scene00::OnInitialize()
{
	physx::PxRigidStatic* plane = physx::PxCreateStatic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(0.0f, -0.5f, 0.0f)),
		physx::PxBoxGeometry(50.0f, 0.5f, 50.0f),
		*m_DefaultMaterial);

	m_Physics->AddBox(plane, QVector3D(0.2f, 0.3f, 0.4f), QVector2D(100.0f, 100.0f));

	for(uint idx = 0; idx < BOXES_COUNT; idx++)
	{
		physx::PxRigidDynamic* box = physx::PxCreateDynamic(
			*m_Physics->GetPhysics(),
			physx::PxTransform(0.0f, 2.5f + ((float)idx * 3.0f), 0.0f, physx::PxQuat((float)idx * 0.2f, physx::PxVec3(1.0f, 0.0f, 0.0f))),
			physx::PxBoxGeometry(0.5f, 0.5f, 0.5f),
			*m_DefaultMaterial,
			10.0f);
		box->setWakeCounter(box->getWakeCounter() * 0.2f);

		m_Physics->AddBox(box, QVector3D(1.0f, 0.8f, 0.6f), QVector2D(1.0f, 1.0f));
	}
}
