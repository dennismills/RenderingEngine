#include "LightManager.h"

LightManager::LightManager()
:	lightCount(0)
{

	memset(&lights[0], 0, sizeof(Light) * MAX_LIGHTS);

	glGenBuffers(1, &uboID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * MAX_LIGHTS, NULL, GL_DYNAMIC_DRAW); // Allocates the buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0); // UNBINDS HERE. Need to rebind before we set the data

	bindingIndex = 0;
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, uboID); // tells us which bidning to put the data in
}

const Light& LightManager::operator[](unsigned int index)
{
	assert(index < MAX_LIGHTS);
	return lights[index];
}

void LightManager::addLight(const Light& light)
{
	assert(lightCount < MAX_LIGHTS);
	assert(sizeof(Light) % 16 == 0);// Forces it to be 16 byte aligned for compliance with GLSL UBOs
	lights[lightCount] = light;
	++lightCount;
}

void LightManager::fillLightBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboID);

	// Uses SubData function to only update a small fraction of the buffer
	// User lightCount because we rarely (if ever) will have MAX_LIGHTS on screen
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Light) * lightCount, &lights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
