#pragma once
#include "ofMain.h"

class MatrixUtils
{
public:
	MatrixUtils();
	~MatrixUtils();
	static glm::mat4 lookAt(glm::vec3 e, glm::vec3 u, glm::vec3 t);
	static glm::mat4 perspective(float fo, float as, float N, float F);
};

