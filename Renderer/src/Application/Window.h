#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Renderer/Renderer.h"

class Window
{
public:
	Window(std::string title, std::size_t width, std::size_t height);
	~Window();
	void startApplication();

private:
	std::string title;
	std::size_t width;
	std::size_t height;

private:
	GLFWwindow* window;
	Renderer* renderer;
};

