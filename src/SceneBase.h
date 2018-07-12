#ifndef SCENEBASE_H
#define SCENEBASE_H

#include "common/PxTolerancesScale.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"

#include "Physics.h"
#include "ISceneProvider.h"

class SceneBase : public QObject, public ISceneProvider
{
	private:

	Q_OBJECT

	protected:

	Physics*           m_Physics;
	physx::PxScene*    m_Scene;
	physx::PxMaterial* m_DefaultMaterial;

	public:

	explicit SceneBase(Physics* physics, QObject* parent = NULL);
	virtual ~SceneBase();

	private: // ISceneProvider implementation

	virtual void Initialize();
	virtual void Deinitialize();

	protected:

	virtual void OnInitialize();
	virtual void OnDeinitialize();
};

#endif // SCENEBASE_H
