#pragma once

#include "../EngineUI/EngineViewport.h"
#include "../Mouse/Mouse.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera()
	{
		position = glm::vec3(0.0);
		target = glm::vec3(0.0);
		up = glm::vec3(0.0);

		firstMouse = false;
		lastX = 0;
		lastY = 0;
		pitch = 0;
		yaw = 0;
	}

	Camera(const glm::vec3& position, const glm::vec3& target);
	glm::mat4 getViewMatrix() const { return viewMatrix; }
	glm::vec3 getPosition() const { return position; }
	void update(GLFWwindow* window);

private:
	double lastX, lastY;
	double yaw, pitch;
	bool firstMouse;

private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	glm::mat4 viewMatrix;
};

