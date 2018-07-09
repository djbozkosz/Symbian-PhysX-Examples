#include "SampleBase.h"

void ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	qDebug() << code << message << file << line;
}

void* AllocatorCallback::allocate(size_t size, const char* typeName, const char* filename, int line)
{
	(void)typeName;
	(void)filename;
	(void)line;

	size_t  alignment    = 16;
	size_t  expandedSize = size + alignment;
	size_t  unaligned    = reinterpret_cast<size_t>(malloc(expandedSize));
	size_t  mask         = (alignment - 1);
	size_t  misalignment = (unaligned & mask);
	size_t  adjustment   = alignment - misalignment;
	size_t  aligned      = unaligned + adjustment;
	uchar*  alignedBytes = reinterpret_cast<uchar*>(aligned);
	alignedBytes[-1]     = static_cast<uchar>(adjustment);

	return reinterpret_cast<void*>(alignedBytes);
}

void AllocatorCallback::deallocate(void* ptr)
{
	uchar*  alignedBytes = reinterpret_cast<uchar*>(ptr);
	size_t  aligned      = reinterpret_cast<size_t>(ptr);
	size_t  adjustment   = static_cast<size_t>(alignedBytes[-1]);
	size_t  unaligned    = aligned - adjustment;

	free(reinterpret_cast<void*>(unaligned));
}

SampleBase::SampleBase(QObject *parent) :
	QObject(parent),
	m_Foundation(NULL),
	m_Physics(NULL),
	m_Cooking(NULL),
	m_Scene(NULL)
{
}

SampleBase::~SampleBase()
{
	m_Timer.stop();

	foreach(actor, m_Actors)
	{
		actor->RigidActor->release();
	}

	m_Scene->release();
	PxCloseExtensions();
	m_Physics->release();
	m_Foundation->release();
}

void SampleBase::Initialize()
{
	m_Foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_AllocatorCallback, m_ErrorCallback);
	m_Physics    = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale());
	m_Cooking    = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(m_Physics->getTolerancesScale()));
	PxInitExtensions(*m_Physics, NULL);

	physx::PxSceneDesc sceneDescriptor(m_Physics->getTolerancesScale());
	sceneDescriptor.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDescriptor.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
	sceneDescriptor.filterShader  = &physx::PxDefaultSimulationFilterShader;
	m_Scene      = m_Physics->createScene(sceneDescriptor);

	m_Timer.setInterval(16);
	connect(&m_Timer, SIGNAL(timeout()), this, SLOT(Simulate()));
	m_Timer.start();
}

void SampleBase::Simulate()
{
	m_Scene->simulate(1.0f / 60.0f);
	m_Scene->fetchResults(true);

	emit Simulated();
}

void SampleBase::AddBox(physx::PxRigidActor* actor, const QVector3D& color, const QVector2D& tiling)
{
	AddActor(actor);
	emit ActorBoxAdded(&m_Actors.back(), color, tiling);
}

void SampleBase::AddMesh(physx::PxRigidActor* actor, const QVector3D& color, const QVector2D& tiling)
{
	AddActor(actor);
	emit ActorMeshAdded(&m_Actors.back(), color, tiling);
}

void SampleBase::AddActor(physx::PxRigidActor *actor)
{
	m_Actors.push_back(Actor(actor));
	m_Scene->addActor(*actor);
}

QMatrix4x4 SampleBase::Actor::GetTransform() const
{
	physx::PxTransform pxTransform = RigidActor->getGlobalPose();
	QMatrix4x4 transform;

	transform.translate(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z);
	transform.rotate(QQuaternion(pxTransform.q.w, pxTransform.q.x, pxTransform.q.y, pxTransform.q.z));

	physx::PxShape *shapes[1];
	if(RigidActor->getShapes(shapes, 1) > 0)
	{
		if(shapes[0]->getGeometryType() == physx::PxGeometryType::eBOX)
		{
			physx::PxBoxGeometry box;
			shapes[0]->getBoxGeometry(box);

			transform.scale(box.halfExtents.x * 2.0f, box.halfExtents.y * 2.0f, box.halfExtents.z * 2.0f);
		}
	}

	return transform;
}

QVector<QVector3D> SampleBase::Actor::GetMeshPositions() const
{
	return QVector<QVector3D>();
}

QVector<QVector3D> SampleBase::Actor::GetMeshNormals() const
{
	return QVector<QVector3D>();
}

QVector<QVector3D> SampleBase::Actor::GetMeshTextureCoords() const
{
	return QVector<QVector3D>();
}

QVector<ushort> SampleBase::Actor::GetIndices() const
{
	return QVector<ushort>();
}
