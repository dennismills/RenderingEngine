#pragma once
#include "ParticleSystem.h"

class FireParticleSystem : public ParticleSystem
{
public:
	FireParticleSystem() {}
	~FireParticleSystem() 
	{
		particles.clear();
	}
	FireParticleSystem(long count, float spawnRate, long ttl);
	
	void serialize() { setName("FireParticleSystem"); }
	void render(GLuint& vao, Shader& shader);
	void update();
	void spawn();
	void populateUI();
};