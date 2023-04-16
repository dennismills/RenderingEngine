#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "../Mesh/Mesh.h"

class Model : public Mesh
{
public:
	Model();
	void rotate(float theta, const glm::vec3& axis);
	void translate(const glm::vec3& amount);
	void scale(const glm::vec3& dim);
	virtual void render(GLuint& vao);

protected:
	bool hasTexture;
};