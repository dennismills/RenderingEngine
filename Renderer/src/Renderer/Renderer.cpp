#include "Renderer.h"
#define DEBUG_TOOLS

Renderer::Renderer(float aspectRatio, float fov, GLFWwindow* window)
:	oldWidth( 0 ),
	oldHeight( 0 ),
	window( window ),
	aspectRatio( aspectRatio ),
	fov( fov ),
	projectionMatrix( glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f) )
{
	initImGui();

	camera = Camera(glm::vec3(0.0, 10.0, 10), glm::vec3(0.0, 0.0, -1.0));

	defaultShader = Shader("src/ShaderPrograms/UBOMain.vert", "src/ShaderPrograms/UBOMain.frag");
	particleShader = Shader("src/ShaderPrograms/particles/particleMain.vert", "src/ShaderPrograms/particles/particleMain.frag");

	initOpenGL(); // Initializes the VAO we need to create the model manager

	models = ModelManager(vao);
	initModels();
	initLights();

	grid = new Grid("src/ShaderPrograms/grid/gridMain.vert", "src/ShaderPrograms/grid/gridMain.frag");
	engineUI.addToSceneEditor(grid);
}

void Renderer::initImGui()
{
	IMGUI_CHECKVERSION();
	ImGuiContext* imGuiContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}

void Renderer::initOpenGL()
{
	glGenVertexArrays(1, &vao);
	if (vao < 0)
	{
		printf("Cannot create VAO\n");
		assert(false);
	}

	glBindVertexArray(vao);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::initModels()
{
	std::shared_ptr<Texture> stallTexture = std::shared_ptr<Texture>(new Texture("src/assets/textures/stallTexture.png"));

	std::shared_ptr<Model> stall = std::shared_ptr<Model>(new ObjModel("src/assets/stall.obj", vao));
	stall->translate(glm::vec3(0, stall->getMin().y, 0));
	stall->setTexture(stallTexture);
	stall->setMaterial({ glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 1.0, 1.0), 4 });

	std::shared_ptr<Model> terrain = std::shared_ptr<Model>(new Terrain(500, 500, vao));
	terrain->translate(glm::vec3(0, 10, 0));
	terrain->rotate(0.5, glm::vec3(1, 0, 0));
	terrain->rotate(2.25, glm::vec3(0, 1, 0));
	terrain->setMaterial({ glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.5, 0.5, 0.5), glm::vec3(1.0, 1.0, 1.0), 4 });

	models.add(terrain);
	models.add(stall);

	for (int i = 0; i < models.size(); ++i)
	{
		engineUI.addToSceneEditor(models[i].get());
	}
	/*ps.loadFromConfig("src/assets/particleSystems/FireParticleSystem.json");*/
}

void Renderer::initLights()
{
	lights.addLight(PointLight(glm::vec3(0.0, -20, -100), glm::vec3(1.0, 0.0, 0.0), glm::vec3(1, 0.09, 0.0000)));
	lights.addLight(DirectionalLight(glm::vec3(0.0, 100, -150), glm::vec3(0.5, 1.0, 1.0)));
}

void Renderer::startImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Renderer::endImGuiFrame()
{
#ifdef DEBUG_TOOLS
	viewport.unbindBuffers();
	viewport.render();
#endif

	ImGui::Render();
	GLFWwindow* backup_current_context = glfwGetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	glfwMakeContextCurrent(backup_current_context);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef DEBUG_TOOLS
	viewport.cleanup();
#endif
}

void Renderer::update(float dt)
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	if (w != oldWidth || oldHeight != h)
	{
		aspectRatio = (float)w / (float)h;
		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
		oldWidth = w;
		oldHeight = h;
	}

	camera.update(window);
	invViewMatrix = glm::inverse(camera.getViewMatrix());

	engineUI.relayKeyPresses(window);
	//ps.update(dt, camera);

	grid->getShader().setUniformMat4(projectionMatrix, "proj");
	grid->getShader().setUniformMat4(glm::inverse(projectionMatrix), "invProj");

	grid->getShader().setUniformMat4(camera.getViewMatrix(), "view");
	grid->getShader().setUniformMat4(invViewMatrix, "invView");

	glm::vec3 cPos = camera.getPosition();
	grid->setPosition(glm::vec3(cPos.x, 0.0, cPos.z));
	grid->getShader().setUniformMat4(grid->getModelMatrix(), "model");
	grid->getShader().setUniformVec2(EngineViewport::getViewportDimensions(), "viewport");
}

void Renderer::composeEngineUIFrame()
{
#ifdef DEBUG_TOOLS
	bool t = true;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	ImGui::SetNextWindowPos(ImVec2(0.0, 0.0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(oldWidth, oldHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;

	ImGui::Begin("DockSpace Demo", &t, window_flags);
	ImGui::PopStyleVar(2);
	ImGui::DockSpace(ImGui::GetID("Dockspace"), ImVec2(0, 0));
	ImGui::End();

	engineUI.createMenuBar();
	engineUI.createLog();
	engineUI.createSceneEditor();
	engineUI.createTextEditor();
	engineUI.createFileTreeExplorer();

	viewport.initBuffers();
#endif
}

void Renderer::renderFrame()
{
	startImGuiFrame();
	composeEngineUIFrame();

	// The block below is the actual rendering block
	// It is in a block so we know where to init the framebuffer
	// and where to unbind the framebuffer for the GUI
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/*particleShader.setUniformMat4(camera.getViewMatrix(), "view");
		particleShader.setUniformMat4(projectionMatrix, "projection");

		ps.render(particleShader);*/

		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		defaultShader.use();
		defaultShader.setUniformMat4(camera.getViewMatrix(), "view");
		defaultShader.setUniformMat4(invViewMatrix, "invView");
		defaultShader.setUniformMat4(projectionMatrix, "projection");
		defaultShader.setUBO(lights, "LightsBlock");
		defaultShader.setUniformVec3(camera.getPosition(), "cameraPos");
		defaultShader.setUniformFloat(Grid::getRenderDistance(), "maxRenderDistance");
		lights.fillLightBuffer();

		for (unsigned int i = 0; i < models.size(); ++i)
		{
			models[i]->rotate(0.5, glm::vec3(0.0, 1.0, 0.0));
		}

		models.render(defaultShader);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisable(GL_BLEND);
		Shader::unbind();

		grid->render();
	}
	endImGuiFrame();
}

void Renderer::killImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

Renderer::~Renderer()
{
	delete grid;
}