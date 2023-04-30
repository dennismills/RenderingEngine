#pragma once
#include <GLFW/glfw3.h>

class Mouse
{
public:
	Mouse() = delete;
	static double getX() { return x; }
	static double getY() { return y; }
	static void mouseCallback(GLFWwindow* window, double xPos, double yPos)
	{
		x = xPos;
		y = yPos;
	}
private:
	static double x, y;
};