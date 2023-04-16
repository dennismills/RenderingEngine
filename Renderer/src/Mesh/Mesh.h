#pragma once
#include <vector>

#include <gl/glew.h>

#include "../Buffers/VertexBuffer.h"
#include "../Vertex/Vertex.h"


class Mesh
{
public:
	Mesh()
	{
		modelMatrix = glm::mat4(1.0);
	}

	void setModelMatrix(const glm::mat4& m) { modelMatrix = m; }
	void add(const Vertex& vertex) { vertices.emplace_back(vertex); }
	
	const std::vector<Vertex>& getVertices() const { return vertices; }
	const Vertex& getVertex(int index) const { return vertices[index]; }
	unsigned int vertexCount() const { return vertices.size(); }

	const glm::mat4& getModelMatrix() const { return modelMatrix; }

private:
	std::vector<Vertex> vertices;

protected:
	glm::mat4 modelMatrix;
	VertexBuffer buffer;
};