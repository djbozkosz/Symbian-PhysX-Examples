#ifndef SCENE02_H
#define SCENE02_H

#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>
#include <PxMaterial.h>
#include <geometry/PxTriangleMesh.h>
#include <geometry/PxTriangleMeshGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <cooking/PxTriangleMeshDesc.h>
#include <extensions/PxDefaultStreams.h>
#include <extensions/PxSimpleFactory.h>

#include "SceneBase.h"

class Scene02 : public SceneBase
{
	private:

	Q_OBJECT

	static const uint SPHERES_COUNT = 50;

	static const physx::PxVec3 VERTICES[];
	static const physx::PxU32 INDICES[];

	QLinkedList<physx::PxRigidDynamic*> m_Spheres;

	public:

	explicit Scene02(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene02();

	private: // SceneBase implementation

	virtual void OnInitialize();
	virtual void OnDeinitialize();
	virtual void OnUpdate();
};

#endif // SCENE02_H
