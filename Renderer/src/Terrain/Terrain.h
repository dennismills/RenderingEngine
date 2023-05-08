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
	void serialize()
	{
		setName("Terrain");
	}
	void populateUI();

	void render(GLuint& vao) { defaultRender(vao); }
private:
	void addTerrainVertex(const Vertex& vertex)
	{
		// Ignores Y axis
		glm::vec2 newPos = glm::vec2(vertex.position.x, vertex.position.z);

		auto it = indexMap.find(newPos);
		if (it != indexMap.end())
		{
			unsigned int index = it->second; // Extract the index for that vertex
			addIndex(index);
		}
		else
		{
			addVertex(vertex);
			unsigned int index = (unsigned int)vertices.size() - 1;
			addIndex(index);
			indexMap.insert(std::pair<glm::vec2, unsigned int>(newPos, index));
		}
	}

private:
	unsigned int width, height;
	unsigned short octaves;
	float persistance;

	std::unordered_map<glm::vec2, unsigned int> indexMap;
};

