#ifndef SCENE02_H
#define SCENE02_H

#include "SceneBase.h"

class Scene02 : public SceneBase
{
	public:

	explicit Scene01(Physics* physics = NULL, QObject* parent = NULL);
	virtual ~Scene01();

	private: // SceneBase implementation

	virtual void OnInitialize();
};

#endif // SCENE02_H
