#ifndef SCENE03_H
#define SCENE03_H

#include "SceneBase.h"

class Scene03 : public SceneBase
{
	private:

	Q_OBJECT

	/*static const uint FUNNEL_VERTICES_COUNT = 32;
	static const uint OBJECTS_COUNT         = 1;

	static float         FUNNEL_VERTICES[];
	static ushort        FUNNEL_INDICES[];
	static physx::PxVec3 FUNNEL_PX_VERTICES[];
	static physx::PxU32  FUNNEL_PX_INDICES[];*/

	public:

	explicit Scene03(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene03();

	private: // SceneBase implementation

	virtual void OnInitialize();
	virtual void OnDeinitialize();
	virtual void OnUpdate();
};

#endif // SCENE03_H
