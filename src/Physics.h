#ifndef PHYSICS_H
#define PHYSICS_H

#include <QTimer>
#include <QElapsedTimer>

#include <QLinkedList>

#include <foundation/PxFoundation.h>
#include <foundation/PxFoundationVersion.h>
#include <foundation/PxErrors.h>
#include <foundation/PxErrorCallback.h>
#include <foundation/PxAllocatorCallback.h>
#include <PxPhysicsVersion.h>
#include <PxPhysics.h>
#include <PxSceneDesc.h>
#include <PxScene.h>
#include <PxRigidDynamic.h>
#include <geometry/PxTriangleMesh.h>
#include <geometry/PxTriangleMeshGeometry.h>
#include <geometry/PxConvexMesh.h>
#include <geometry/PxConvexMeshGeometry.h>
#include <cooking/PxCooking.h>
#include <cooking/PxTriangleMeshDesc.h>
#include <cooking/PxConvexMeshDesc.h>
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxSimpleFactory.h>
#include <extensions/PxDefaultStreams.h>

#include "ISceneProvider.h"
#include "ISceneObjectProvider.h"

class ErrorCallback : public physx::PxErrorCallback
{
	virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
};

class AllocatorCallback : public physx::PxAllocatorCallback
{
	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line);
	virtual void  deallocate(void* ptr);
};

class IPhysicsSceneProvider : public ISceneProvider
{
	public:

	virtual physx::PxScene* GetScene() const = 0;
};

class Physics : public QObject
{
	private:

	Q_OBJECT

	static const float DELTA_MIN;
	static const float DELTA_MAX;

	public:

	class Actor : public ISceneObjectProvider
	{
		public:

		physx::PxRigidActor* RigidActor;

		Actor(physx::PxRigidActor* rigidActor) : RigidActor(rigidActor) {}

		private: // ISceneObjectProvider implementation

		virtual QMatrix4x4      GetTransform() const;
		virtual QVector<float>  GetVertices()  const { return QVector<float>();  }
		virtual QVector<ushort> GetIndices()   const { return QVector<ushort>(); }
	};

	private:

	physx::PxFoundation*   m_Foundation;
	physx::PxPhysics*      m_Physics;
	physx::PxCooking*      m_Cooking;

	IPhysicsSceneProvider* m_ActiveScene;

	QLinkedList<Actor *>   m_Actors;

	QTimer                 m_Timer;
	QElapsedTimer          m_Elapsed;
	float                  m_DeltaTime;
	float                  m_AdditionalDelta;

	uint64_t               m_ElapsedTime;
	uint                   m_FrameCounter;

	ErrorCallback          m_ErrorCallback;
	AllocatorCallback      m_AllocatorCallback;

	public:

	explicit Physics(QObject *parent = NULL);
	virtual ~Physics();

	inline physx::PxPhysics*           GetPhysics()   const { return m_Physics;            }
	inline physx::PxCooking*           GetCooking()   const { return m_Cooking;            }
	inline physx::PxAllocatorCallback* GetAllocator()       { return &m_AllocatorCallback; }
	inline void SetActiveScene(IPhysicsSceneProvider* scene) { m_ActiveScene = scene; }

	physx::PxTriangleMesh* CreateMesh      (const QVector<float> &vertices, const QVector<ushort> &indices);
	physx::PxConvexMesh*   CreateConvexMesh(const QVector<float> &vertices, const QVector<ushort> &indices);

	void AddBox  (physx::PxRigidActor *actor, const QVector4D &color, const QVector2D &tiling);
	void AddSpere(physx::PxRigidActor *actor, const QVector4D &color, const QVector2D &tiling);
	void AddMesh (Actor *actor, const QVector4D &color, const QVector2D &tiling);

	void CleanActors();

	public slots:

	void Initialize();
	void SetAdditionalDelta(float delta);

	signals:

	void Simulated();

	void ActorBoxAdded   (const ISceneObjectProvider* actor, const QVector4D& color, const QVector2D& tiling);
	void ActorSphereAdded(const ISceneObjectProvider* actor, const QVector4D& color, const QVector2D& tiling);
	void ActorMeshAdded  (const ISceneObjectProvider* actor, const QVector4D& color, const QVector2D& tiling);

	void StatsUpdated_SimulateMs(float simulateMs);
	void StatsUpdated_AwakeActors(uint awakeActors);

	private slots:

	void Simulate();

	private:

	void UpdateFallens();

	void AddActor(Actor *actor);
};

#endif // PHYSICS_H
