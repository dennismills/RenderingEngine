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

		bool hasUVCoords = false;

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
						hasUVCoords = true;
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

			if (hasUVCoords)
			{
				unsigned int uvIndex = uvIndices[i];
				uv = tempUVs[uvIndex - 1];

				// Blender uses a different UV coordinate system than OpenGL
				// so I do 1 - uv.y to convert it to the OpenGL
				// system so it gets rendered properly.

				// TODO: Add a way to detect inverted UV coords and only invert when needed
				uv = glm::vec2(uv.x, 1.0 - uv.y);
			}
			else
			{
				uv = glm::vec2(0.0, 0.0);
			}
			addOBJVertex({ vertex, normal, uv });
		}

		populateBuffers();
		clearIndexMap();
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

void ObjModel::populateUI()
{
	bool transformationTreeNode = ImGui::TreeNodeEx("Transformation",
		ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth,
		"Transformation");

	if (transformationTreeNode)
	{
		// modelMatrix[3] is the position glm::vec3 in the matrix. We pass address 0 of that vector
		ImGui::SliderFloat3("Translation", &modelMatrix[3][0], -300, 300);
		ImGui::TreePop();
	}



	bool materialTreeNode = ImGui::TreeNodeEx("Material Properties",
		ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth,
		"Material"
	);

	if (materialTreeNode)
	{
		ImGui::SliderFloat("Shininess", &material.shininess, 0.5, 8);
		ImGui::ColorPicker3("Ambient", &material.ambient[0]);
		ImGui::ColorPicker3("Diffuse", &material.diffuse[0]);
		ImGui::ColorPicker3("Specular", &material.specular[0]);
		ImGui::TreePop();
	}
}