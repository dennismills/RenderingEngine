#pragma once
#include <GLFW/glfw3.h>

class Mouse
{
public:
	Mouse() = delete;
	static double getX() { return x; }
	static double getY() { return y; }
	static bool isLeftPressed() { return leftClicked; }
	static void mouseCallback(GLFWwindow* window, double xPos, double yPos)
	{
		x = xPos;
		y = yPos;
	}
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			leftClicked = true;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			leftClicked = false;
		}
	}
private:
	static double x, y;
	static bool leftClicked;
};