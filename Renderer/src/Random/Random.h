#pragma once
#include <random>
#include <cassert>

class Random
{
public:
	Random() = delete;

	static void seed(unsigned long time)
	{
		srand(time);
	}
	static float random(float min, float max)
	{
		assert(max > min);
		return (min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))));
	}
	static glm::vec3 randomVec3(glm::vec3 min, glm::vec3 max)
	{
		return glm::vec3
		(
			random(min.x, max.x),
			random(min.y, max.y),
			random(min.z, max.z)
		);
	}
};