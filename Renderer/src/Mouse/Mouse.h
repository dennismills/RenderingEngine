#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <unordered_map>
#include <string>

#include "../Bounds/BoundingRect.h"

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
		for (auto& r : rects)
		{
			if (r.second.isInside(xPos, yPos)) { return; }
		}
	}
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		for (auto& r : rects)
		{
			if (r.second.isInside(x, y))
			{
				return;
			}
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			leftClicked = true;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			leftClicked = false;
		}
	}
	static void addBoundingRect(std::string name, const BoundingRect& r)
	{
		if (rects.find(name) == rects.end())
		{
			rects.insert(std::pair<std::string, BoundingRect>(name, r));
		}
		else
		{
			rects[name] = r;
		}
	}

private:
	static double x, y;
	static bool leftClicked;
	static std::unordered_map<std::string, BoundingRect> rects;
};