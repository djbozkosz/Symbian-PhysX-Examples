#include "Physics.h"

const float Physics::DELTA_MIN = 1.0f / 60.0f;
const float Physics::DELTA_MAX = 1.0f / 5.0f;

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

Physics::Physics(QObject *parent) :
	QObject(parent),
	m_Foundation(NULL),
	m_Physics(NULL),
	m_Cooking(NULL),
	m_ActiveScene(NULL),
	m_DeltaTime(DELTA_MIN),
	m_ElapsedTime(0UL),
	m_FrameCounter(0)
{
}

Physics::~Physics()
{
	m_Timer.stop();

	PxCloseExtensions();
	m_Physics->release();
	m_Foundation->release();
}

void Physics::Initialize()
{
	m_Foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_AllocatorCallback, m_ErrorCallback);
	m_Physics    = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale());
	m_Cooking    = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(m_Physics->getTolerancesScale()));
	PxInitExtensions(*m_Physics, NULL);

#ifdef Q_OS_SYMBIAN
	m_Timer.setInterval(0);
#else
	m_Timer.setInterval((int)(m_DeltaTime * 1000.0f));
#endif
	connect(&m_Timer, SIGNAL(timeout()), this, SLOT(Simulate()));
	m_Timer.start();
	m_Elapsed.start();
}

void Physics::AddBox(physx::PxRigidActor* actor, const QVector4D& color, const QVector2D& tiling)
{
	AddActor(actor);
	emit ActorBoxAdded(&m_Actors.back(), color, tiling);
}

void Physics::AddSpere(physx::PxRigidActor *actor, const QVector4D &color, const QVector2D &tiling)
{
	AddActor(actor);
	emit ActorSphereAdded(&m_Actors.back(), color, tiling);
}

void Physics::AddMesh(physx::PxRigidActor* actor, const QVector4D& color, const QVector2D& tiling)
{
	AddActor(actor);
	emit ActorMeshAdded(&m_Actors.back(), color, tiling);
}

void Physics::CleanActors()
{
	foreach(actor, m_Actors)
	{
		actor->RigidActor->release();
	}

	m_Actors.clear();
}

void Physics::Simulate()
{
	if(m_ActiveScene == NULL)
		return;

	uint64_t timeStart = m_Elapsed.elapsed();

	m_ActiveScene->GetScene()->simulate(m_DeltaTime);
	m_ActiveScene->GetScene()->fetchResults(true);

	m_ActiveScene->Update();
	UpdateFallens();

	uint64_t timeEnd   = m_Elapsed.elapsed();
	uint64_t timeDelta = timeEnd - timeStart;
	m_DeltaTime        = timeDelta * 0.001f;
	m_ElapsedTime     += timeDelta;
	m_FrameCounter++;

	if(m_DeltaTime < DELTA_MIN)
	{
		m_DeltaTime = DELTA_MIN;
	}
	else if(m_DeltaTime > DELTA_MAX)
	{
		m_DeltaTime = DELTA_MAX;
	}

	if(m_ElapsedTime > 1000)
	{
		emit StatsUpdated_SimulateMs(m_ElapsedTime / m_FrameCounter);

		m_ElapsedTime  = 0UL;
		m_FrameCounter = 0;

		uint awakeActors = 0;

		foreach(actor, m_Actors)
		{
			physx::PxRigidDynamic* dynamicActor = actor->RigidActor->is<physx::PxRigidDynamic>();

			if(dynamicActor != NULL && dynamicActor->isSleeping() == false)
			{
				awakeActors++;
			}
		}

		emit StatsUpdated_AwakeActors(awakeActors);
	}

	emit Simulated();
}

void Physics::UpdateFallens()
{
	foreach(actor, m_Actors)
	{
		physx::PxRigidDynamic* dynamicActor = actor->RigidActor->is<physx::PxRigidDynamic>();

		if(dynamicActor != NULL && dynamicActor->isSleeping() == false)
		{
			physx::PxTransform transform = dynamicActor->getGlobalPose();

			if(transform.p.y > -10.0f)
				continue;

			transform.p = physx::PxVec3(0.0f, 50.0f, 0.0f);
			dynamicActor->setGlobalPose(transform);
			dynamicActor->setLinearVelocity(physx::PxVec3(physx::PxZero));
		}
	}
}

void Physics::AddActor(physx::PxRigidActor *actor)
{
	m_Actors.push_back(Actor(actor));
	m_ActiveScene->GetScene()->addActor(*actor);
}

QMatrix4x4 Physics::Actor::GetTransform() const
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
		else if(shapes[0]->getGeometryType() == physx::PxGeometryType::eSPHERE)
		{
			physx::PxSphereGeometry sphere;
			shapes[0]->getSphereGeometry(sphere);

			transform.scale(sphere.radius);
		}
	}

	return transform;
}

QVector<QVector3D> Physics::Actor::GetMeshPositions() const
{
	return QVector<QVector3D>();
}

QVector<QVector3D> Physics::Actor::GetMeshNormals() const
{
	return QVector<QVector3D>();
}

QVector<QVector3D> Physics::Actor::GetMeshTextureCoords() const
{
	return QVector<QVector3D>();
}

QVector<ushort> Physics::Actor::GetIndices() const
{
	return QVector<ushort>();
}
