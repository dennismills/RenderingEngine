#include "Renderer.h"


Renderer::Renderer(float fov)
:	fov(fov)
{
	defaultShader = Shader("src/ShaderPrograms/UBOMain.vert", "src/ShaderPrograms/UBOMain.frag");
	viewMatrix = glm::mat4(1.0);
	invViewMatrix = glm::inverse(viewMatrix);
	projectionMatrix = glm::perspective(glm::radians(45.0f), fov, 0.1f, 100.0f);
	glGenVertexArrays(1, &vao);
	if (vao < 0)
	{
		printf("Cannot create VAO\n");
		assert(false);
	}
	glBindVertexArray(vao);

	//model = ObjModel("src/assets/stall.obj", vao);
	//model.translate(glm::vec3(0, -3, -20));

	terrain = Terrain(10, 10, vao);
	terrain.translate(glm::vec3(-5.0, -3.0, -150));
	terrain.scale(glm::vec3(10, 10, 10));

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	
	lights.addLight(DirectionalLight(glm::vec3(0.0, 5.0, -20.0), glm::vec3(0.5, 1.0, 1.0)));

}

void Renderer::renderFrame()
{
	glClearColor(0.25f, 0.0f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	defaultShader.use();
	defaultShader.setUniformMat4(terrain.getModelMatrix(), "model");
	defaultShader.setUniformMat4(viewMatrix, "view");
	defaultShader.setUniformMat4(invViewMatrix, "invView");
	defaultShader.setUniformMat4(projectionMatrix, "projection");
	defaultShader.setUBO(lights, "LightsBlock");
	lights.fillLightBuffer();

	//model.rotate(0.0025f, glm::vec3(0.0f, 1.0f, 0.0f));
	//model.render(vao);
	terrain.rotate(0.01f, glm::vec3(0.0, 1.0f, 0.0f));
	terrain.render(vao);
	Shader::unbind();
}