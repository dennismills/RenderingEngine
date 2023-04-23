#pragma once
#include "Particle.h"
#include "../Shader/Shader.h"
#include "../Model/BatchModel.h"
#include "../Random/Random.h"
#include "../nlohmann/json.hpp"
#include  "../JsonUtils/JsonUtils.h"

class ParticleSystem
{
public:
	ParticleSystem() {}
	void setTexture(Texture* texture)
	{
		if (batch == nullptr)
		{
			printf("Trying to set texture before batch is initialized.\n");
			assert(false);
		}
		batch->setTexture(texture);
	}
	virtual void update() = 0;
	virtual void render(GLuint& vao, Shader& shader) = 0;
	virtual void spawn() = 0;

	void loadFromConfig(const std::string& fileName)
	{
		std::ifstream file(fileName);
		if (!file)
		{
			printf("Cannot open file: %s\n", fileName.c_str());
			assert(false);
		}

		config = nlohmann::json::parse(file);
		setSpawnPosition(JsonUtils::vectorToVec3(config["minPosition"]), JsonUtils::vectorToVec3(config["maxPosition"]));
		setSpawnScale(JsonUtils::vectorToVec3(config["minScale"]), JsonUtils::vectorToVec3(config["maxScale"]));
		maxParticles = config["maxParticles"];
		ttl = config["ttl"];
		spawnRate = config["spawnRate"];

		batch = new BatchModel<Particle>();

		if (config.find("textureSrc") != config.end())
		{
			setTexture(new Texture(config["textureSrc"]));
		}
	}

	void setSpawnPosition(const glm::vec3& min, const glm::vec3& max)
	{
		minPos = min;
		maxPos = max;
	}
	void setSpawnRotation(const glm::vec3& min, const glm::vec3& max)
	{
		minRot = min;
		maxRot = max;
	}
	void setSpawnScale(const glm::vec3& min, const glm::vec3& max)
	{
		minScale = min;
		maxScale = max;
	}

	BatchModel<Particle>* getModel() { return batch; }

protected:
	long maxParticles;
	float spawnRate;
	long ttl;

protected:
	nlohmann::json config;

	std::vector<Particle> particles;

	glm::vec3 minPos, maxPos;
	glm::vec3 minScale, maxScale;
	glm::vec3 minRot, maxRot;

	Timer spawnTimer;
	BatchModel<Particle>* batch;
};