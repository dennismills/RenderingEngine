#pragma once
#include "Model.h"

template <typename T> class BatchModel : public Model
{
public:
	BatchModel<T>()
	{}

	~BatchModel<T>()
	{
		printf("Batch deleted\n");
	}

	void render(GLuint& vao)
	{
		if (vertices.size() > 0 && indices.size() > 0)
		{
			defaultRender(vao);
		}
	}

	void fromVector(std::vector<T> objs)
	{
		vertices.clear();
		indices.clear();
		clearIndexMap();

		if (objs.size() > 0)
		{
			hasTexture = true;
		}
		for (T& t : objs)
		{
			std::vector<Vertex> verts = t.getVertices();
			for (auto& v : verts)
			{
				glm::vec3 newPosition = glm::vec3(t.getModelMatrix() * glm::vec4(v.position, 1.0));
				glm::vec3 newNormal = glm::vec3(t.getModelMatrix() * glm::vec4(v.normal, 1.0));
				Vertex newVertex = { newPosition, newNormal, v.uv };
				addOBJVertex(newVertex);
			}
		}
	}

private:
};

