#pragma once

#include "../Shader/Shader.h"
#include "../Buffers/VertexBuffer.h"
#include "../Model/ObjModel.h"
#include "../Lighting/LightManager.h"
#include "../Terrain/Terrain.h"
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(float fov);
	void renderFrame();

private:
	float fov;
	Shader defaultShader;
	glm::mat4 modelMatrix, viewMatrix, projectionMatrix, invViewMatrix;

private:
	LightManager lights;
	float modelRot = 0.0f;
	unsigned int vao;
	//ObjModel model;
	Terrain terrain;
};

