#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;

	static const unsigned int VERTEX_POS = 0;
	static const unsigned int NORMAL_POS = sizeof(glm::vec3);
	static const unsigned int UV_POS = sizeof(glm::vec3) + sizeof(glm::vec3);
};