#ifndef SCENE01_H
#define SCENE01_H

#include <foundation/PxMat44.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>
#include <PxMaterial.h>
#include <geometry/PxPlaneGeometry.h>
#include <geometry/PxBoxGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <extensions/PxJoint.h>
#include <extensions/PxSphericalJoint.h>

#include "SceneBase.h"

class Scene01 : public SceneBase
{
	private:

	Q_OBJECT

	static const uint BOXES_COUNT_HORIZONTAL = 6;
	static const uint BOXES_COUNT_VERTICAL   = 4;

	public:

	explicit Scene01(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene01();

	private: // SceneBase implementation

	virtual void OnInitialize();

	void SetupWall(QVector3D position);
};

#endif // SCENE01_H
