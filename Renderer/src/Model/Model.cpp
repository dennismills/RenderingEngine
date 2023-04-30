#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"

Model::Model()
{
	hasTexture = false;
}

void Model::setTexture(std::shared_ptr<Texture> texture)
{
	this->texture = texture;
}

void Model::rotate(float theta, const glm::vec3& axis)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(theta), axis);
}

void Model::translate(const glm::vec3& amount)
{
	position += amount;
	modelMatrix = glm::translate(modelMatrix, amount);
}

void Model::scale(const glm::vec3& dim)
{
	modelMatrix = glm::scale(modelMatrix, dim);
}