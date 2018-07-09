#ifndef SAMPLEBASE_H
#define SAMPLEBASE_H

#include <QTimer>

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
#include "common/PxTolerancesScale.h"
#include "cooking/PxCooking.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxSimpleFactory.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"

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

class SampleBase : public QObject
{
	private:

	Q_OBJECT

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

	QLinkedList<Actor>   m_Actors;

	protected:

	physx::PxFoundation* m_Foundation;
	physx::PxPhysics*    m_Physics;
	physx::PxCooking*    m_Cooking;

	physx::PxScene*      m_Scene;

	private:

	QTimer               m_Timer;

	ErrorCallback        m_ErrorCallback;
	AllocatorCallback    m_AllocatorCallback;

	public:

	explicit SampleBase(QObject *parent = NULL);
	virtual ~SampleBase();

	public slots:

	virtual void Initialize();

	signals:

	void Simulated();

	void ActorBoxAdded(const ISceneObjectProvider* actor, const QVector3D& color, const QVector2D& tiling);
	void ActorMeshAdded(const ISceneObjectProvider* actor, const QVector3D& color, const QVector2D& tiling);

	protected:

	void AddBox(physx::PxRigidActor *actor, const QVector3D &color, const QVector2D &tiling);
	void AddMesh(physx::PxRigidActor *actor, const QVector3D &color, const QVector2D &tiling);

	private slots:

	void Simulate();

	private:

	void AddActor(physx::PxRigidActor *actor);
};

#endif // SAMPLEBASE_H
