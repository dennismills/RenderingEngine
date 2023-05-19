#include "Terrain.h"

Terrain::Terrain(const unsigned int width, const unsigned int height, unsigned int& vao)
:	width(width),
	height(height)
{
    octaves = 1;
    persistance = 0.5f;
    float scale = 1.0f;

    int vertsX = 256;
    int vertsZ = 256;
    float stepsizeX = (float)width / vertsX;
    float stepsizeZ = (float)height / vertsZ;

    float frequency = 0.01589f; // adjust this to change the frequency of the Perlin noise
    float amplitude = 25.0f; // adjust this to change the amplitude of the Perlin noise


    glm::vec3 normal = glm::vec3(0.0);

    for (int i = 0; i < vertsX; ++i)
    {
        for (int j = 0; j < vertsZ; ++j)
        {
            // Calculate the noise value for this vertex using glm::perlin
            float noiseValue = glm::perlin(glm::vec3(i * frequency, 0.0f, j * frequency));

            // Calculate the Y coordinate for this vertex using the noise value and the amplitude
            float vertexY = noiseValue * amplitude;

            // Triangle 1
            glm::vec3 vertex1Position = glm::vec3(i * stepsizeX, vertexY, j * stepsizeZ);
            glm::vec3 vertex2Position = glm::vec3((i * stepsizeX), vertexY, (j * stepsizeZ) + stepsizeZ);
            glm::vec3 vertex3Position = glm::vec3((i * stepsizeX) + stepsizeX, vertexY, (j * stepsizeZ) + stepsizeZ);

            glm::vec3 edge1 = vertex2Position - vertex1Position;
            glm::vec3 edge2 = vertex3Position - vertex1Position;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            addTerrainVertex({ vertex1Position, normal, glm::vec2(0.0, 0.0) });
            addTerrainVertex({ vertex2Position, normal, glm::vec2(0.0, 0.0) });
            addTerrainVertex({ vertex3Position, normal, glm::vec2(0.0, 0.0) });

            // Triangle 2
            glm::vec3 vertex4Position = glm::vec3((i * stepsizeX) + stepsizeX, vertexY, (j * stepsizeZ) + stepsizeZ);
            glm::vec3 vertex5Position = glm::vec3((i * stepsizeX) + stepsizeX, vertexY, j * stepsizeZ);
            glm::vec3 vertex6Position = glm::vec3(i * stepsizeX, vertexY, j * stepsizeZ);

            edge1 = vertex5Position - vertex4Position;
            edge2 = vertex6Position - vertex4Position;
            normal = glm::normalize(glm::cross(edge1, edge2));

            addTerrainVertex({ vertex4Position, normal, glm::vec2(0.0, 0.0) });
            addTerrainVertex({ vertex5Position, normal, glm::vec2(0.0, 0.0) });
            addTerrainVertex({ vertex6Position, normal, glm::vec2(0.0, 0.0) });
        }
    }

    populateBuffers();
}

void Terrain::populateUI()
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