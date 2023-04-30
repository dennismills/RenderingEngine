#pragma once

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include "../Mesh/Mesh.h"
#include "../Texture/Texture.h"

class Model : public Mesh
{
public:
	Model();
	void setTexture(std::shared_ptr<Texture> texture);
	void rotate(float theta, const glm::vec3& axis);
	void translate(const glm::vec3& amount);
	void scale(const glm::vec3& dim);
	virtual void render(GLuint& vao) = 0;

	const bool textured() const { return hasTexture && texture->getPixels() != nullptr; }
	std::shared_ptr<Texture> getTexture() const { return texture; }

protected:
	void defaultRender(GLuint& vao)
	{
		glBindVertexArray(vao);
		bindIBO();
		bindVBO();
		bindTexture();

		// Casting to GLsizei to reduce warnings in the code about implicit size conversions
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
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
	std::shared_ptr<Texture> texture;
	bool hasTexture;
};