#include "Window.h"


double Mouse::x = 0;
double Mouse::y = 0;
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

Window::Window(std::string title, int width, int height)
:	
	title(title),
	width(width),
	height(height)
{
    Random::seed((unsigned long)time(NULL));
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Could not initialize GLFW. Application is exiting" << std::endl;
        return;
    }

    /* Create a windowed mode window and its OpenGL context */
    if (width < 0 || height < 0)
    {
        glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
        this->window = glfwCreateWindow(1, 1, this->title.c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        this->window = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);
    }
    
    glfwSetWindowSize(window, width, height);
    if (!this->window)
    {
        std::cout << "Could not create window context from GLFW. Application is exiting" << std::endl;
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Could not initialize GLEW" << std::endl;
        return;
    }
    glfwSwapInterval(1);

    // During init, enable debug output
    /*glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);*/

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, Mouse::mouseCallback);
    float fov = (float)width / (float)height;
    if (width < 0 || height < 0)
    {
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        fov = (float)w / (float)h;
    }
    renderer = new Renderer(fov, window);

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });
}

Window::~Window()
{
    delete renderer;
}

void Window::startApplication()
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(this->window))
    {
        renderer->update();
        renderer->renderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(this->window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    renderer->killImGui();
    glfwTerminate();
}