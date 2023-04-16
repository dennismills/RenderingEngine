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
		glGenBuffers(1, &ibo);
		if (ibo < 0)
		{
			printf("Cannot create IBO in Mesh.h\n");
			assert(false);
		}
	}

	void add(const Vertex& vertex)
	{
		auto it = indexMap.find(vertex.position);
		if (it != indexMap.end())
		{
			unsigned int index = it->second; // Extract the index for that vertex
			indices.push_back(index);
		}
		else
		{
			vertices.emplace_back(vertex);
			unsigned int index = (unsigned int)vertices.size() - 1;
			indices.push_back(index);
			indexMap.insert(std::pair<glm::vec3, unsigned int>(vertex.position, index));
		}

	}
	void bindVBO() { buffer.bind(); }
	void bindIBO() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); }

	const glm::mat4& getModelMatrix() const { return modelMatrix; }


	void populateBuffers(GLuint& vao)
	{
		buffer = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::VERTEX_POS);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)Vertex::NORMAL_POS);
		glEnableVertexAttribArray(1);

		bindIBO();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}


private:
	VertexBuffer buffer;
	std::unordered_map<glm::vec3, unsigned int> indexMap;
	unsigned int ibo;
protected:
	glm::mat4 modelMatrix;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};