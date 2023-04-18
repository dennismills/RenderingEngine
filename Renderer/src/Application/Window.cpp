#include "Window.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

Window::Window(std::string title, std::size_t width, std::size_t height)
:	
	title(title),
	width(width),
	height(height)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Could not initialize GLFW. Application is exiting" << std::endl;
        return;
    }

    /* Create a windowed mode window and its OpenGL context */
    this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
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

    renderer = new Renderer((float)width / (float)height, window);

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
        renderer->renderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(this->window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    renderer->killImGui();
    glfwTerminate();
}