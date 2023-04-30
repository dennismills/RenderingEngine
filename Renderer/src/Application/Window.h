#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Renderer/Renderer.h"
#include "../Random/Random.h"
#include "../Mouse/Mouse.h"

class Window
{
public:
	Window(std::string title, int width, int height);
	~Window();
	void startApplication();

private:
	std::string title;
	int width;
	int height;

private:
	GLFWwindow* window;
	Renderer* renderer;
};

