#include "FireParticleSystem.h"

FireParticleSystem::FireParticleSystem(long count, float spawnRate, long ttl)
{
	this->maxParticles = count;
	this->spawnRate = spawnRate;
}

void FireParticleSystem::render(GLuint& vao, Shader& shader)
{
	for (int i = 0; i < particles.size(); ++i)
	{
		Particle p = particles[i];
		shader.setModelProperties(p);
		if (p.isAlive())
		{
			p.populateBuffers();
			shader.setUniformMat4(p.getModelMatrix(), "model");
			p.render(vao);
		}
	}
}

void FireParticleSystem::update()
{
	spawn();
	for (auto& p : particles)
	{
		p.update(spawnTimer.currentTime());
	}

	particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle& p)
	{
		return !p.isAlive();
	}), particles.end());
}

void FireParticleSystem::spawn()
{
	if (this->particles.size() >= maxParticles)
	{
		spawnTimer.reset();
		return;
	}

	float spawnInterval = 1000.0f / spawnRate;
	if (spawnTimer.elapsed() > spawnInterval)
	{
		spawnTimer.reset();
		Particle p(3000);

		int randomX = 20 - (rand() % 40);
		int randomY = 20 - (rand() % 20);
		int randomZ = -120 - (rand() % 10);

		p.translate(glm::vec3(randomX, randomY, randomZ));
		p.scale(glm::vec3(10.0, 12.0, 1.0));
		p.setTTL(5000);
		p.setTexture(texture);

		particles.push_back(p);
	}
}