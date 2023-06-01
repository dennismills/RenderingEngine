#pragma once
#include <glm/glm.hpp>
#include <vector>

class JsonUtils
{
public:
	JsonUtils() = delete;
	static glm::vec3 vectorToVec3(std::vector<float> vec)
	{
		assert(vec.size() >= 3);
		if (vec.size() > 3) { printf("Vector has size > 3. Values will be truncated.\n"); }
		glm::vec3 output(vec[0], vec[1], vec[2]);
		return output;
	}

	static glm::vec2 vectorToVec2(std::vector<float> vec)
	{
		assert(vec.size() >= 2);
		if (vec.size() > 2) { printf("Vector has size > 2. Values will be truncated.\n"); }
		glm::vec2 output(vec[0], vec[1]);
		return output;
	}

private:
};