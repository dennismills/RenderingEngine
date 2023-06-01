#pragma once
#include <glm/glm.hpp>


// TODO: In the future we need to make sure that we pass the Material along with the
// Vertex information on a per vertex basis. Each vertex should be able to have a material
// property to have multi-material models. For now, each model gets a single material
struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};