#include "Renderer.h"


Renderer::Renderer(float fov, GLFWwindow* window)
:	fov(fov)
{

	IMGUI_CHECKVERSION();
	ImGuiContext* imGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	defaultShader = Shader("src/ShaderPrograms/UBOMain.vert", "src/ShaderPrograms/UBOMain.frag");
	viewMatrix = glm::mat4(1.0);
	invViewMatrix = glm::inverse(viewMatrix);
	projectionMatrix = glm::perspective(glm::radians(45.0f), fov, 0.1f, 1000.0f);
	glGenVertexArrays(1, &vao);
	if (vao < 0)
	{
		printf("Cannot create VAO\n");
		assert(false);
	}

	glBindVertexArray(vao);

	/*model = ObjModel("src/assets/stall.obj", vao);
	model.translate(glm::vec3(0, -3, -20));*/

	terrain = Terrain(100, 100, vao);
	terrain.translate(glm::vec3(0, -20, -100));
	terrain.rotate(0.5, glm::vec3(1, 0, 0));
	terrain.rotate(2.25, glm::vec3(0, 1, 0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	//lights.addLight(DirectionalLight(glm::vec3(0.0, 50.0, -150), glm::vec3(0.5, 1.0, 1.0)));
	lights.addLight(PointLight(glm::vec3(0.0, -20, -100), glm::vec3(1.0, 0.0, 0.0), glm::vec3(1, 0.09, 0.0000)));

}

void Renderer::startImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::endImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::renderFrame()
{
	startImGuiFrame();
	ImGui::SliderFloat("X", (float*) & lights[0].position.x, -50, 50);
	ImGui::SliderFloat("Y", (float*)&lights[0].position.y, -40, 0);
	ImGui::SliderFloat("Z", (float*)&lights[0].position.z, -150, 150);
	glClearColor(0.25f, 0.0f, 0.75f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	defaultShader.use();
	defaultShader.setUniformMat4(terrain.getModelMatrix(), "model");
	defaultShader.setUniformMat4(viewMatrix, "view");
	defaultShader.setUniformMat4(invViewMatrix, "invView");
	defaultShader.setUniformMat4(projectionMatrix, "projection");
	defaultShader.setUBO(lights, "LightsBlock");
	lights.fillLightBuffer();

	/*model.rotate(0.0025f, glm::vec3(0.0f, 1.0f, 0.0f));
	model.render(vao);*/
	//terrain.rotate(0.01f, glm::vec3(0.0, 1.0f, 0.0f));
	terrain.render(vao);
	Shader::unbind();
	endImGuiFrame();
}

void Renderer::killImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}