#ifndef SCENE03_H
#define SCENE03_H

#include "Scene02.h"

class Scene03 : public Scene02
{
	private:

	Q_OBJECT

	static const uint OBJECTS_COUNT = 1;

	public:

	explicit Scene03(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene03();

	protected: // Scene02 implementation

	void OnInitializeObjects();
};

#endif // SCENE03_H
