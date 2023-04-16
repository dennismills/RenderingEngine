#include "Terrain.h"

Terrain::Terrain(const unsigned int width, const unsigned int height, unsigned int& vao)
:	width(width),
	height(height)
{
    octaves = 1;
    persistance = 0.5f;
    float scale = 1.0f;

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            float noiseValue00, noiseValue10, noiseValue01, noiseValue11;
            glm::vec3 normal00, normal10, normal01, normal11;
            glm::vec2 gradient00, gradient10, gradient01, gradient11;

            // Calculate noise values and normals for the four corners of the cell
            gradient00 = gradient();
            noiseValue00 = glm::perlin(glm::vec3(glm::vec2(i, j) / scale, 1.0f));
            normal00 = glm::normalize(glm::vec3(glm::perlin(glm::vec3(glm::vec2(i, j) / scale, 1.0f))));

            gradient10 = gradient();
            noiseValue10 = glm::perlin(glm::vec3(glm::vec2(i + 1, j) / scale, 1.0f));
            normal10 = glm::normalize(glm::vec3(glm::perlin(glm::vec3(glm::vec2(i + 1, j) / scale, 1.0f))));

            gradient01 = gradient();
            noiseValue01 = glm::perlin(glm::vec3(glm::vec2(i, j + 1) / scale, 1.0f));
            normal01 = glm::normalize(glm::vec3(glm::perlin(glm::vec3(glm::vec2(i, j + 1) / scale, 1.0f))));

            gradient11 = gradient();
            noiseValue11 = glm::perlin(glm::vec3(glm::vec2(i + 1, j + 1) / scale, 1.0f));
            normal11 = glm::normalize(glm::vec3(glm::perlin(glm::vec3(glm::vec2(i + 1, j + 1) / scale, 1.0f))));

            // Add the first triangle
            Vertex vertex00, vertex10, vertex01;

            vertex00.position = glm::vec3(i, noiseValue00 * height, j);
            vertex00.normal = normal00;
            vertex00.uv = glm::vec2(i, j);
            add(vertex00);

            vertex10.position = glm::vec3(i + 1, noiseValue10 * height, j);
            vertex10.normal = normal10;
            vertex10.uv = glm::vec2(i + 1, j);
            add(vertex10);

            vertex01.position = glm::vec3(i, noiseValue01 * height, j + 1);
            vertex01.normal = normal01;
            vertex01.uv = glm::vec2(i, j + 1);
            add(vertex01);

            // Add the second triangle
            Vertex vertex11;
            vertex11.position = glm::vec3(i + 1, noiseValue11 * height, j + 1);
            vertex11.normal = normal11;
            vertex11.uv = glm::vec2(i + 1, j + 1);
            add(vertex11);

            // Reuse vertex10 and vertex
            add(vertex9);
            add(vertex11);
            add(vertex10);
        }
    }
    populateVBO(vao);
}