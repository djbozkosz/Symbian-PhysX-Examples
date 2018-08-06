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

	static const uint FUNNEL_VERTICES_COUNT = 32;
	static const uint SPHERES_COUNT         = 50;
	static const uint BOXES_COUNT           = 50;

	static float         FUNNEL_VERTICES[];
	static ushort        FUNNEL_INDICES[];
	static physx::PxVec3 FUNNEL_PX_VERTICES[];
	static physx::PxU32  FUNNEL_PX_INDICES[];

	class Funnel : public Physics::Actor
	{
		public:

		Funnel(physx::PxRigidActor* rigidActor) : Physics::Actor(rigidActor) {}

		private: // ISceneObjectProvider implementation

		virtual QVector<float>  GetVertices() const;
		virtual QVector<ushort> GetIndices()  const;
	};

	QLinkedList<physx::PxRigidDynamic*> m_Objects;
	physx::PxMaterial* m_BoxMaterial;

	public:

	explicit Scene02(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene02();

	private: // SceneBase implementation

	virtual void OnInitialize();
	virtual void OnDeinitialize();
	virtual void OnUpdate();
};

#endif // SCENE02_H
