#ifndef SCENE00_H
#define SCENE00_H

#include "foundation/PxMat44.h"
#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxMaterial.h"
#include "geometry/PxPlaneGeometry.h"
#include "geometry/PxBoxGeometry.h"

#include "SceneBase.h"

class Scene00 : public SceneBase
{
	private:

	Q_OBJECT

	static const uint      BOXES_COUNT = 100;

	public:

	explicit Scene00(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene00();

	private: // SceneBase implementation

	virtual void OnInitialize();
};

#endif // SCENE00_H
