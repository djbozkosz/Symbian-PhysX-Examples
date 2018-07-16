#ifndef PHYSICS_H
#define PHYSICS_H

#include <QTimer>
#include <QElapsedTimer>

#include <QLinkedList>

#include <foundation/PxFoundation.h>
#include "foundation/PxFoundationVersion.h"
#include "foundation/PxErrors.h"
#include "foundation/PxErrorCallback.h"
#include "foundation/PxAllocatorCallback.h"
#include "PxPhysicsVersion.h"
#include "PxPhysics.h"
#include "PxSceneDesc.h"
#include "PxScene.h"
#include "cooking/PxCooking.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxSimpleFactory.h"

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

class Physics : public QObject
{
	private:

	Q_OBJECT

	float DELTA_MIN;
	float DELTA_MAX;

	protected:

	class Actor : public ISceneObjectProvider
	{
		public:

		physx::PxRigidActor* RigidActor;

		Actor(physx::PxRigidActor* rigidActor) : RigidActor(rigidActor) {}

		private: // ISceneObjectProvider implementation

		virtual QMatrix4x4 GetTransform()                 const;
		virtual QVector<QVector3D> GetMeshPositions()     const;
		virtual QVector<QVector3D> GetMeshNormals()       const;
		virtual QVector<QVector3D> GetMeshTextureCoords() const;
		virtual QVector<ushort>    GetIndices()           const;
	};

	physx::PxFoundation* m_Foundation;
	physx::PxPhysics*    m_Physics;
	physx::PxCooking*    m_Cooking;

	physx::PxScene*      m_ActiveScene;

	QLinkedList<Actor>   m_Actors;

	private:

	QTimer               m_Timer;
	QElapsedTimer        m_Elapsed;
	float                m_DeltaTime;

	ErrorCallback        m_ErrorCallback;
	AllocatorCallback    m_AllocatorCallback;

	public:

	explicit Physics(QObject *parent = NULL);
	virtual ~Physics();

	inline physx::PxPhysics* GetPhysics() const { return m_Physics; }
	inline void SetActiveScene(physx::PxScene* scene) { m_ActiveScene = scene; }

	void AddBox  (physx::PxRigidActor *actor, const QVector4D &color, const QVector2D &tiling);
	void AddSpere(physx::PxRigidActor *actor, const QVector4D &color, const QVector2D &tiling);
	void AddMesh (physx::PxRigidActor *actor, const QVector4D &color, const QVector2D &tiling);

	void CleanActors();

	public slots:

	virtual void Initialize();

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

	void AddActor(physx::PxRigidActor *actor);
};

#endif // PHYSICS_H
