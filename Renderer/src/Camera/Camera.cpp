#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& target)
{
	this->position = position;
	this->target = target;
	this->firstMouse = false;
	this->yaw = -90.0f;
	this->pitch = 3.0f;

	viewMatrix = glm::mat4(1.0);
	cameraDirection = glm::normalize(position - target);

	up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	viewMatrix = glm::lookAt(position, target, up);
}

void Camera::update(GLFWwindow* window)
{
	const float cameraSpeed = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += target * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= target * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= glm::normalize(glm::cross(target, up)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += glm::normalize(glm::cross(target, up)) * cameraSpeed;
	}

	if (firstMouse)
	{
		lastX = Mouse::getX();
		lastY = Mouse::getY();
		firstMouse = false;
	}

	double xoffset = Mouse::getX() - lastX;
	double yoffset = lastY - Mouse::getY();
	lastX = Mouse::getX();
	lastY = Mouse::getY();

	double sensitivity = 0.25f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;

	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = (double)cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = (double)sin(glm::radians(pitch));
	direction.z = (double)sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	target = glm::normalize(direction);

	viewMatrix = glm::lookAt(position, position + target, up);
}