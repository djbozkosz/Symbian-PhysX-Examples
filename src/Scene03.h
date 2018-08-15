#ifndef SCENE03_H
#define SCENE03_H

#include "Scene02.h"

class Scene03 : public Scene02
{
	private:

	Q_OBJECT

	static const uint DIAMOND_VERTICES_COUNT = 10;
	static const uint DIAMONDS_COUNT         = 50;

	class Diamond : public Physics::Actor
	{
		public:

		Diamond(physx::PxRigidActor* rigidActor) : Physics::Actor(rigidActor) {}

		private: // ISceneObjectProvider implementation

		virtual QVector<float>  GetVertices() const { return GlConstants::GetDiamond(DIAMOND_VERTICES_COUNT)->Vertices; }
		virtual QVector<ushort> GetIndices()  const { return GlConstants::GetDiamond(DIAMOND_VERTICES_COUNT)->Indices;  }
	};

	public:

	explicit Scene03(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene03();

	protected: // Scene02 implementation

	void OnInitializeObjects();
};

#endif // SCENE03_H
