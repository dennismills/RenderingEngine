#pragma once

#include "../Shader/Shader.h"
#include "../Buffers/VertexBuffer.h"
#include "../Model/ObjModel.h"
#include "../Lighting/LightManager.h"
#include "../Terrain/Terrain.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(float fov, GLFWwindow* window);
	void renderFrame();
	void killImGui();

private:
	void startImGuiFrame();
	void endImGuiFrame();

private:
	float fov;
	Shader defaultShader;
	glm::mat4 modelMatrix, viewMatrix, projectionMatrix, invViewMatrix;

private:
	LightManager lights;
	unsigned int vao;
	//ObjModel model;
	Terrain terrain;
};

