#include "Model.h"

Model::Model()
{

}

void Model::rotate(float theta, const glm::vec3& axis)
{
	modelMatrix = glm::rotate(modelMatrix, theta, axis);
}

void Model::translate(const glm::vec3& amount)
{
	modelMatrix = glm::translate(modelMatrix, amount);
}

void Model::scale(const glm::vec3& dim)
{
	modelMatrix = glm::scale(modelMatrix, dim);
}

void Model::render(GLuint& vao)
{
	glBindVertexArray(vao);
	buffer.bind();
	glDrawArrays(GL_TRIANGLES, 0, vertexCount());
}