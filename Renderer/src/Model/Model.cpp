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
	bindIBO();
	bindVBO();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}