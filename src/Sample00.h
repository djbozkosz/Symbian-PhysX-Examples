#ifndef SAMPLE00_H
#define SAMPLE00_H

#include "foundation/PxMat44.h"
#include "PxRigidStatic.h"
#include "PxRigidDynamic.h"
#include "PxMaterial.h"
#include "geometry/PxPlaneGeometry.h"
#include "geometry/PxBoxGeometry.h"

#include "SampleBase.h"

class Sample00 : public SampleBase
{
	private:

	Q_OBJECT

	protected:

	static const uint      BOXES_COUNT = 10;

	physx::PxMaterial*     m_Material;

	public:

	explicit Sample00(QObject *parent = NULL);
	virtual ~Sample00();

	public slots:

	virtual void Initialize();
};

#endif // SAMPLE00_H
