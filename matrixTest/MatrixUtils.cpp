#include "MatrixUtils.h"



MatrixUtils::MatrixUtils()
{
}


MatrixUtils::~MatrixUtils()
{
}

glm::mat4 MatrixUtils::lookAt(glm::vec3 e, glm::vec3 u, glm::vec3 t) {
	glm::mat4 dest;
	glm::vec3 eye = e;
	glm::vec3 up = u;
	glm::vec3 target = t;
	float x0, x1, x2, y0, y1, y2, z0, z1, z2;
	z0 = eye[0] - target[0];
	z1 = eye[1] - target[1];
	z2 = eye[2] - target[2];
	float l = 1.0 / sqrtf(pow(z0, 2) + pow(z1, 2) + pow(z2, 2));
	z0 *= l;
	z1 *= l;
	z2 *= l;

	x0 = up[1] * z2 - up[2] * z1;
	x1 = up[2] * z0 - up[0] * z2;
	x2 = up[0] * z1 - up[1] * z0;
	l = 1.0 / sqrtf(pow(x0, 2) + pow(x1, 2) + pow(x2, 2));
	x0 *= l;
	x1 *= l;
	x2 *= l;

	y0 = z1 * x2 - z2 * x1;
	y1 = z2 * x0 - z0 * x2;
	y2 = z0 * x1 - z1 * x0;
	l = 1.0 / sqrtf(pow(y0, 2) + pow(y1, 2) + pow(y2, 2));
	y0 *= l;
	y1 *= l;
	y2 *= l;

	glm::vec4 x = glm::vec4(x0, x1, x2, 0);
	glm::vec4 y = glm::vec4(y0, y1, y2, 0);
	glm::vec4 z = glm::vec4(z0, z1, z2, 0);
	glm::vec4 w = glm::vec4(-(x0 * eye[0] + x1 * eye[1] + x2 * eye[2]),
		-(y0 * eye[0] + y1 * eye[1] + y2 * eye[2]),
		-(z0 * eye[0] + z1 * eye[1] + z2 * eye[2]),
		1.0);
	dest = glm::mat4(x, y, z, w);

	return dest;
}
glm::mat4 MatrixUtils::perspective(float fo, float as, float N, float F) {
	glm::mat4 dest;
	float fov = fo;
	float aspect = as;
	float Near = N;
	float Far = F;

	float t = Near * tan(fov * PI / 360.0);
	float r = t * aspect;
	float a = r * 2.0;
	float b = t * 2.0;
	float c = Far - Near;
	glm::vec4 x = glm::vec4(Near * 2 / a, 0.0, 0.0, 0.0);
	glm::vec4 y = glm::vec4(0.0, Near * 2 / b, 0.0, 0.0);
	glm::vec4 z = glm::vec4(0.0, 0.0, -(Far + Near) / c, -1.0);
	glm::vec4 w = glm::vec4(0.0, 0.0, -(Far * Near * 2) / c, 0.0);
	dest = glm::mat4(x, y, z, w);
	return dest;
}