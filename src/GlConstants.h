#ifndef GLMESHES_H
#define GLMESHES_H

#include <QtGlobal>

class GlConstants
{
	public:

	static const char* VERTEX_SHADER;
	static const char* FRAGMENT_SHADER;

	static const uint   CUBE_VERTICES_COUNT = 24;
	static const uint   CUBE_INDICES_COUNT  = 12;
	static const float  CUBE_VERTICES[];
	static const ushort CUBE_INDICES[];

	static const uint   SPHERE_VERTICES_COUNT = 234;
	static const uint   SPHERE_INDICES_COUNT  = 256;
	static const float  SPHERE_VERTICES[];
	static const ushort SPHERE_INDICES[];
};

#endif // GLMESHES_H
