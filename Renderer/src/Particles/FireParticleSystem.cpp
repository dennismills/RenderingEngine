#include "FireParticleSystem.h"

FireParticleSystem::FireParticleSystem(long count, float spawnRate, long ttl)
{
	this->maxParticles = count;
	this->spawnRate = spawnRate;
	this->ttl = ttl;
	batch = std::make_shared<BatchModel<Particle>>();
}

void FireParticleSystem::render(GLuint& vao, Shader& shader)
{
	batch->fromVector(particles);
	batch->render(vao);
}

void FireParticleSystem::update()
{
	spawn();
	particles.erase(std::remove_if(particles.begin(), particles.end(), [](Particle& p)
		{
			return !p.isAlive();
		}), particles.end());


	for (auto& p : particles)
	{
		p.update(spawnTimer.currentTime());

		glm::vec3 minTrans = JsonUtils::vectorToVec3(config["minTranslationPerFrame"]);
		glm::vec3 maxTrans = JsonUtils::vectorToVec3(config["maxTranslationPerFrame"]);

		p.translate(Random::randomVec3(minTrans, maxTrans));
	}


	std::sort(particles.begin(), particles.end(),
		[&](const Particle& p1, const Particle& p2)
		{
			float distance1 = glm::distance(glm::vec3(0, 0, 0), p1.getPosition());
			float distance2 = glm::distance(glm::vec3(0, 0, 0), p2.getPosition());
			return distance1 > distance2;
		});
}

void FireParticleSystem::spawn()
{
	if (this->particles.size() >= maxParticles)
	{
		spawnTimer.reset();
		return;
	}

	
	for (int i = 0; i < spawnRate; ++i)
	{
		Particle p(ttl);

		glm::vec3 position = glm::vec3
		(
			Random::random(minPos.x, maxPos.x),
			Random::random(minPos.y, maxPos.y),
			Random::random(minPos.z, maxPos.z)
		);

		//glm::vec3 rotation = glm::vec3
		//(
		//	random::random(minrot.x, maxrot.x),
		//	random::random(minrot.y, maxrot.y),
		//	random::random(minrot.z, maxrot.z)
		//);

		glm::vec3 scale = glm::vec3
		(
			Random::random(minScale.x, maxScale.x),
			Random::random(minScale.y, maxScale.y),
			Random::random(minScale.z, maxScale.z)
		);

		p.translate(position);
		p.scale(scale);
		//p.rotate(rotation);
		p.setTTL(this->ttl);
		particles.push_back(p);
	}
}

void FireParticleSystem::populateUI()
{

}