#ifndef ISCENEOBJECTPROVIDER_H
#define ISCENEOBJECTPROVIDER_H

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <QVector>

#undef foreach
#define foreach(it, list) for(auto it = list.begin(); it != list.end(); it++)

class ISceneObjectProvider
{
	public:

	virtual ~ISceneObjectProvider() {}

	virtual QMatrix4x4 GetTransform()                 const = 0;
	virtual QVector<QVector3D> GetMeshPositions()     const = 0;
	virtual QVector<QVector3D> GetMeshNormals()       const = 0;
	virtual QVector<QVector3D> GetMeshTextureCoords() const = 0;
	virtual QVector<ushort>    GetIndices()           const = 0;
};

#endif // ISCENEOBJECTPROVIDER_H
