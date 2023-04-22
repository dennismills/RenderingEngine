#pragma once
#include "../Shader/Shader.h"
#include "Particle.h"

class ParticleSystem
{
public:
	ParticleSystem() {}
	void setTexture(Texture* texture)
	{
		this->texture = texture;
	}
	virtual void update() = 0;
	virtual void render(GLuint& vao, Shader& shader) = 0;
	virtual void spawn() = 0;
protected:
	Texture* texture;
	Timer spawnTimer;
	long maxParticles;
	float spawnRate;
	std::vector<Particle> particles;
};