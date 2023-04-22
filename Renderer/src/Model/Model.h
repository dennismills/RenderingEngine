#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "../Mesh/Mesh.h"
#include "../Texture/Texture.h"

class Model : public Mesh
{
public:
	Model();
	void setTexture(Texture* texture);
	void rotate(float theta, const glm::vec3& axis);
	void translate(const glm::vec3& amount);
	void scale(const glm::vec3& dim);
	virtual void render(GLuint& vao) = 0;

	const bool textured() const { return hasTexture && texture->getPixels() != nullptr; }


protected:
	void defaultRender(GLuint& vao)
	{
		glBindVertexArray(vao);
		bindIBO();
		bindVBO();
		bindTexture();

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void bindTexture()
	{
		if (hasTexture)
		{
			texture->bind();
		}
	}

protected:
	Texture* texture;
	bool hasTexture;
};