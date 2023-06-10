#pragma once

#include "../Shader/Shader.h"
#include "../Buffers/VertexBuffer.h"
#include "../Model/ObjModel.h"
#include "../Lighting/LightManager.h"
#include "../Terrain/Terrain.h"
#include "../Model/ModelManager.h"
#include "../Particles/ParticleSystem.h"
#include "../Camera/Camera.h"
#include "../EngineUI/EngineUI.h"
#include "../EngineUI/EngineViewport.h"
#include "../EngineUI/Grid/Grid.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(float fov, GLFWwindow* window);
	~Renderer();
	void renderFrame();
	void update(float dt);
	void killImGui();

private:
	void initImGui();
	void initOpenGL();
	void initModels();
	void initLights();

	void startImGuiFrame();
	void endImGuiFrame();
	void composeEngineUIFrame(); // For engine UI debugging

private:
	Shader defaultShader, particleShader;
	glm::mat4 projectionMatrix, invViewMatrix;
	unsigned int vao;
	unsigned int oldWidth, oldHeight;
	GLFWwindow* window;

private:
	Camera camera;
	LightManager lights;
	ModelManager models;
	EngineViewport viewport;
	EngineUI engineUI;
	Grid* grid;

	//ParticleSystem ps;
};

