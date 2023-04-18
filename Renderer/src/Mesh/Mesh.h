#pragma once
#include <vector>
#include <unordered_map>
#include <gl/glew.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#include "../Buffers/VertexBuffer.h"
#include "../Vertex/Vertex.h"


class Mesh
{
public:
	Mesh()
	{
		modelMatrix = glm::mat4(1.0);

		glGenBuffers(1, &vbo);
		if (vbo < 0)
		{
			std::cout << "Could not create the VertexBuffer object: id = " << vbo << std::endl;
		}

		glGenBuffers(1, &ibo);
		if (ibo < 0)
		{
			printf("Cannot create IBO in Mesh.h\n");
			assert(false);
		}
	}

	// Used for OBJ models where we don't have index data
	void addOBJVertex(const Vertex& vertex)
	{
		auto it = indexMap.find(vertex.position);
		if (it != indexMap.end())
		{
			unsigned int index = it->second; // Extract the index for that vertex
			indices.push_back(index);
		}
		else
		{
			vertices.push_back(vertex);
			unsigned int index = (unsigned int)vertices.size() - 1;
			indices.push_back(index);
			indexMap.insert(std::pair<glm::vec3, unsigned int>(vertex.position, index));
		}
	}
	
	// Used when we can get the index data manually
	void addVertex(const Vertex& vertex) { vertices.push_back(vertex); }
	void addIndex(const unsigned int index) { indices.push_back(index); }

	void bindVBO() { glBindBuffer(GL_ARRAY_BUFFER, vbo); }
	void bindIBO() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); }

	void populateBuffers()
	{
		bindVBO();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::VERTEX_POS);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::NORMAL_POS);

		bindIBO();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
	}
	void clearIndexMap() { indexMap.clear(); }

	const glm::mat4& getModelMatrix() const { return modelMatrix; }

private:
	std::unordered_map<glm::vec3, unsigned int> indexMap;
	unsigned int ibo;
	unsigned int vbo;
protected:
	glm::mat4 modelMatrix;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};