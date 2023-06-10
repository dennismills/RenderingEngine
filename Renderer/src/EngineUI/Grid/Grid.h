#pragma once
#include <string>
#include <vector>

#include "../../Shader/Shader.h"

class Grid : public GameObject
{
public:
    Grid(const std::string& vertexShader, const std::string& fragmentShader)
    {
        lineColor = glm::vec3(1.0, 1.0, 1.0);
        s = Shader(vertexShader, fragmentShader);
        model = glm::mat4(1.0);

        glGenBuffers(1, &vao);
        assert(vao >= 0);

        glGenBuffers(1, &vbo);
        assert(vbo >= 0);

        float scale = 500;

        float startX = -scale / 2.0f; // Starting X coordinate
        float endX = scale / 2.0f;    // Ending X coordinate
        float startZ = -scale / 2.0f; // Starting Z coordinate
        float endZ = scale / 2.0f;    // Ending Z coordinate
        float spacing = 5.0f;  // Spacing between grid lines

        // Generate the vertices for the grid lines
        for (float x = startX; x <= endX; x += spacing) {
            vertices.push_back(x);   // X coordinate
            vertices.push_back(0.0f); // Y coordinate (always 0 in XZ plane)
            vertices.push_back(startZ);
            vertices.push_back(x);
            vertices.push_back(0.0f);
            vertices.push_back(endZ);
        }

        for (float z = startZ; z <= endZ; z += spacing) {
            vertices.push_back(startX);
            vertices.push_back(0.0f);
            vertices.push_back(z);
            vertices.push_back(endX);
            vertices.push_back(0.0f);
            vertices.push_back(z);
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Bind the VAO and VBO
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Upload vertex data to the VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        // Configure vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void serialize()
    {
        setName("Grid");
    }
    void populateUI()
    {
        ImGui::DragFloat("Render Distance", &maxRenderDistance, 0.5, 1.0, 500.0f);
        ImGui::ColorPicker3("Line Color", &lineColor[0]);
    }
    void setPosition(const glm::vec3& c) { model[3] = glm::vec4(c, 1.0); }
    void render()
    {
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        s.use();
        s.setUniformVec3(lineColor, "lineColor");
        s.setUniformFloat(maxRenderDistance, "maxRenderDistance");

        // Enable line smoothing
        glEnable(GL_LINE_SMOOTH);

        // Set the desired line width
        float lineWidth = 3.0f;
        glLineWidth(lineWidth);
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);

        Shader::unbind();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
        glEnable(GL_CULL_FACE);
    }

    Shader& getShader() { return s; }
    const glm::mat4& getModelMatrix() const { return model; }
    static float getRenderDistance() { return maxRenderDistance; }
    ~Grid()
    {
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

private:
    Shader s;
    unsigned int vao;
    unsigned int vbo;
    std::vector<GLfloat> vertices;
    glm::mat4 model;
    glm::vec3 lineColor;

private:
    static float maxRenderDistance;
};
