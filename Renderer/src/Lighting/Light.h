#pragma once
#include <glm/glm.hpp>

enum LightType
{
	DIRECTIONAL,
	POINT,
	SPOTLIGHT
};

#define DirectionalLight(pos, color)    \
		{\
			glm::vec4(pos, 0.0),\
			glm::vec4(color, 0.0),\
			glm::vec4(0.0),\
			glm::vec4(0.0),\
            glm::vec4(0.0),\
		    glm::vec4(LightType::DIRECTIONAL)\
		}

#define PointLight(pos, color, falloff)    \
		{\
			glm::vec4(pos, 0.0),\
			glm::vec4(color, 0.0),\
			glm::vec4(falloff, 0.0),\
			glm::vec4(0.0),\
            glm::vec4(0.0),\
		    glm::vec4(LightType::POINT)\
		}

#define SpotLight(pos, color, falloff, direction, cutoff)    \
		{\
			glm::vec4(pos, 0.0),\
			glm::vec4(color, 0.0),\
			glm::vec4(falloff, 0.0),\
			glm::vec4(direction, 0.0),\
            glm::vec4(glm::cos(glm::radians((float)cutoff))),\
		    glm::vec4(LightType::SPOTLIGHT)\
		}

struct Light
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 falloff;
	glm::vec4 direction;
	glm::vec4 cutoff;
	glm::vec4 type;
};