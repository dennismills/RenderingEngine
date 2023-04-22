#pragma once
#include "ParticleSystem.h"

class FireParticleSystem : public ParticleSystem
{
public:
	FireParticleSystem() {}
	~FireParticleSystem() 
	{
		printf("Destroying fire particle system\n");
		particles.clear();
	}
	FireParticleSystem(long count, float spawnRate, long ttl);
	void render(GLuint& vao, Shader& shader);
	void update();
	void spawn();
};