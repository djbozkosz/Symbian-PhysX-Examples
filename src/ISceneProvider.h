#ifndef ISCENEPROVIDER_H
#define ISCENEPROVIDER_H

class ISceneProvider
{
	public:

	virtual ~ISceneProvider() {}

	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;
};

#endif // ISCENEPROVIDER_H
