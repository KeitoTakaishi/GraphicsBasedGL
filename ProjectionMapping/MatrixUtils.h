#pragma once
#include "ofMain.h"

class MatrixUtils
{
public:
	MatrixUtils();
	~MatrixUtils();
	glm::mat4 lookAt(glm::vec3 e, glm::vec3 u, glm::vec3 t);
	glm::mat4 perspective(float fo, float as, float N, float F);
	glm::mat4 textureMatrix();
};