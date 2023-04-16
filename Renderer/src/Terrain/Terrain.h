#pragma once

#include <random>
#include <cmath>
#include <glm/gtc/noise.hpp>
#include "../Model/Model.h"

class Terrain : public Model
{
public:
	Terrain() {}
	Terrain(const unsigned int width, const unsigned int height, unsigned int& vao);

private:
	glm::vec2 gradient()
	{
		std::default_random_engine generator;
		std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		float angle = distribution(generator) * 2 * 3.14159;
		return glm::vec2(cos(angle), sin(angle));
	}

private:
	unsigned int width, height;
	unsigned short octaves;
	float persistance;
};

