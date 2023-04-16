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
	void populateVBO(GLuint& vao)
	{
		buffer = VertexBuffer(&getVertex(0), getVertices().size() * sizeof(Vertex));
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::VERTEX_POS);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::NORMAL_POS);
		glEnableVertexAttribArray(1);
	}

protected:
	bool hasTexture;
};