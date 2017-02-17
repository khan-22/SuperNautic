#include "mathConverterFuncs.h"

glm::vec3 toGLM(aiVector3D vec)
{
	return{ vec.x, vec.y, vec.z };
}

glm::mat4 toGLM(aiMatrix4x4 mat)
{
	return glm::mat4{
		mat.a1, mat.b1, mat.c1, mat.d1,
		mat.a2, mat.b2, mat.c2, mat.d2,
		mat.a3, mat.b3, mat.c3, mat.d3,
		mat.a4, mat.b4, mat.c4, mat.d4
	};
}
