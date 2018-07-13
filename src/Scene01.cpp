#include "Scene01.h"

Scene01::Scene01(Physics* physics, QObject* parent) :
	SceneBase(physics, parent)
{
}

Scene01::~Scene01()
{
}

void Scene01::OnInitialize()
{
	physx::PxRigidStatic* plane = physx::PxCreateStatic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(0.0f, -0.5f, 0.0f)),
		physx::PxBoxGeometry(50.0f, 0.5f, 50.0f),
		*m_DefaultMaterial);

	m_Physics->AddBox(plane, QVector3D(0.2f, 0.3f, 0.4f), QVector2D(100.0f, 100.0f));

	SetupWall(QVector3D(5.0f, 0.0f, 0.0f));
	SetupWall(QVector3D(-2.0f, 0.0f, 0.0f));

	/*physx::PxRigidStatic* hang = physx::PxCreateStatic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(4.0f, 8.0f, 0.0f)),
		physx::PxSphereGeometry(0.5f),
		*m_DefaultMaterial);*/

	physx::PxRigidDynamic* sphere = physx::PxCreateDynamic(
		*m_Physics->GetPhysics(),
		physx::PxTransform(physx::PxVec3(15.0f, 4.0f, 0.0f)),
		physx::PxSphereGeometry(1.5f),
		*m_DefaultMaterial,
		100.0f);
	sphere->setLinearVelocity(physx::PxVec3(-15.0f, 0.0f, 0.0f));

	m_Physics->AddSpere(sphere, QVector3D(0.9f, 0.2f, 0.2f), QVector2D(1.0f, 1.0f));

	/*physx::PxSphericalJoint* joint = physx::PxSphericalJointCreate(*m_Physics->GetPhysics(),
		hang,   physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),
		sphere, physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)));*/
}

void Scene01::SetupWall(QVector3D position)
{
	for(uint idxVertical = 0; idxVertical < BOXES_COUNT_VERTICAL; idxVertical++)
	{
		for(uint idxHorizontal = 0; idxHorizontal < BOXES_COUNT_HORIZONTAL; idxHorizontal++)
		{
			physx::PxRigidDynamic* box = physx::PxCreateDynamic(
				*m_Physics->GetPhysics(),
				physx::PxTransform(
					position.x() + (float)(rand() % 1000) * 0.0001f,
					position.y() + 0.5f + (float)idxVertical,
					position.z() + (float)BOXES_COUNT_HORIZONTAL * -0.5f + (float)idxHorizontal + (float)(rand() % 1000) * 0.0001f,
					physx::PxQuat((float)(rand() % 1000) * 0.00002f - 0.00001f, physx::PxVec3(0.0f, 1.0f, 0.0f))),
				physx::PxBoxGeometry(0.5f, 0.5f, 0.5f),
				*m_DefaultMaterial,
				10.0f);
			box->putToSleep();

			m_Physics->AddBox(box, QVector3D(1.0f, 0.8f, 0.6f), QVector2D(1.0f, 1.0f));
		}
	}
}
