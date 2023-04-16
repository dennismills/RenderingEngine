#include "ObjModel.h"

ObjModel::ObjModel(const std::string& fileName, unsigned int& vao)
{
	hasTexture = false;
	std::ifstream file(fileName);
	if (!file) { std::cout << "Cannot open the file: " << fileName << std::endl; }
	else
	{
		std::vector<glm::vec3> tempVerts, tempNorms;
		std::vector<glm::vec2> tempUVs;

		std::vector<int> vertexIndices, uvIndices, normalIndices;

		std::string temp;
		while (std::getline(file, temp))
		{
			if (temp.size() == 0) { continue; } // Skip empty lines
			std::vector<std::string> tokens = split(temp, " ");
			if (tokens[0] == "v")
			{
				glm::vec3 vertex;
				vertex.x = std::stof(tokens[1]);
				vertex.y = std::stof(tokens[2]);
				vertex.z = std::stof(tokens[3]);
				tempVerts.push_back(vertex);
			}
			else if (tokens[0] == "vt")
			{
				glm::vec2 uv;
				uv.x = std::stof(tokens[1]);
				uv.y = std::stof(tokens[2]);
				tempUVs.push_back(uv);
			}
			else if (tokens[0] == "vn")
			{
				glm::vec3 normal;
				normal.x = std::stof(tokens[1]);
				normal.y = std::stof(tokens[2]);
				normal.z = std::stof(tokens[3]);
				tempNorms.push_back(normal);
			}
			else if (tokens[0] == "f")
			{
				assert(tokens.size() == 4); // Not triangulated otherwise
				for (int i = 1; i < tokens.size(); i++)
				{
					unsigned int vertIndex, uvIndex, normalIndex;
					std::vector<std::string> subTokens = split(tokens[i], "/");
					if (subTokens.size() == 3)
					{
						hasTexture = true;
						vertIndex = std::stoi(subTokens[0]);
						uvIndex = std::stoi(subTokens[1]);
						normalIndex = std::stoi(subTokens[2]);

						vertexIndices.push_back(vertIndex);
						uvIndices.push_back(uvIndex);
						normalIndices.push_back(normalIndex);
					}
					if (subTokens.size() == 2)
					{
						vertIndex = std::stoi(subTokens[0]);
						normalIndex = std::stoi(subTokens[1]);

						vertexIndices.push_back(vertIndex);
						normalIndices.push_back(normalIndex);
					}
				}
			}
			else 
			{
				// This could be material processing and things like that
				// Skip for now
				continue;
			}
		}
		file.close();

		for (int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertIndex   = vertexIndices[i];
			unsigned int normalIndex = normalIndices[i];

			glm::vec3 vertex = tempVerts[vertIndex - 1];
			glm::vec3 normal = tempNorms[normalIndex - 1];

			glm::vec2 uv;

			if (hasTexture)
			{
				unsigned int uvIndex = uvIndices[i];
				uv = tempUVs[uvIndex - 1];
			}
			else
			{
				uv = glm::vec2(0.0, 0.0);
			}
			add({ vertex, normal, uv });
		}

		populateVBO(vao);
	}
}

// TODO:
//	If I parallelize this I need to not use strtok. It is not thread-safe
std::vector<std::string> ObjModel::split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> output;
	char* token = strtok((char*)str.c_str(), delim.c_str());
	while (token != nullptr)
	{
		output.push_back(token);
		token = strtok(nullptr, delim.c_str());
	}
	return output;
}