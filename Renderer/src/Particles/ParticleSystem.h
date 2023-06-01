#pragma once
#include "Particle.h"
#include "../Shader/Shader.h"
#include "../Model/BatchModel.h"
#include "../Random/Random.h"
#include "../nlohmann/json.hpp"
#include  "../JsonUtils/JsonUtils.h"

#include "../Camera/Camera.h"

#include <chrono>

class ParticleSystem : public GameObject
{
public:
	ParticleSystem()
	{
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		vertices.push_back(1.0f);
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);

		vertices.push_back(0.0f);
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);

		vertices.push_back(1.0f);
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);
	}

	~ParticleSystem()
	{
		assert(positions != nullptr);
		delete[] positions;

		assert(uvs != nullptr);
		delete[] uvs;
	}

	void serialize()
	{
		setName("Particle System");
	}

	void populateUI()
	{

	}

	void setTexture(std::shared_ptr<Texture> texture)
	{
		this->texture = texture;
	}

	void update(float dt, const Camera& camera)
	{
		spawn();
		for (int i = 0; i < particles.size(); ++i)
		{
			
			particles[i].position[1] -= Random::random(1.0, 20.0) * dt / 1000;
			particles[i].lifetime -= dt;
			if (particles[i].lifetime > 0)
			{
				float elapsed = (float)particles[i].initialLifetime - particles[i].lifetime;
				float percent = elapsed / particles[i].initialLifetime;

				printf("%f\n", percent);

				int currentFrameIndex = int(percent * cellSize.x);
				int startX = currentFrameIndex * cellSize.x;
				int endX = startX + (cellSize.x - 1); // The - 1 comes from the fact that the first pixel is 0, not 1.

				float startU = (float)startX / textureSize.x; // Normalize between 0 - 1
				float endU = (float)endX / textureSize.x; // Normalize between 0 - 1

				float startY = 0.0;
				float endY = cellSize.y / textureSize.y;

				uvs[i * 2 * 4] = startU;
				uvs[i * 2 * 4 + 1] = startY;
				uvs[i * 2 * 4 + 2] = endU;
				uvs[i * 2 * 4 + 3] = startY;
				uvs[i * 2 * 4 + 4] = startU;
				uvs[i * 2 * 4 + 5] = endY;
				uvs[i * 2 * 4 + 6] = endU;
				uvs[i * 2 * 4 + 7] = endY;
			}

			positions[i * 4 + 0] = particles[i].position[0];
			positions[i * 4 + 1] = particles[i].position[1];
			positions[i * 4 + 2] = particles[i].position[2];
			positions[i * 4 + 3] = particles[i].lifetime;
		}
	}

	void render(Shader& shader)
	{
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(4);

		glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 4 * sizeof(float), positions);

		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 2 * 4 * sizeof(float), uvs);

		// uv buffer
		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glVertexAttribDivisor(1, 0);

		// position buffer
		glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glVertexAttribDivisor(4, 1);

		texture->bind();

		shader.use();
		// draw triangles
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());

		Shader::unbind();

		glVertexAttribDivisor(4, 0);
		glVertexAttribDivisor(1, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	// Note: The number of max particles should be kept low
	// in the json file, as we recycle them here. If it is high then
	// we can clog bandwith to the GPU
	void spawn()
	{
		long capacity = maxParticles - particles.size();
		if (particles.size() < maxParticles)
		{
			for (int i = 0; i < std::min(spawnRate, capacity); ++i)
			{
				Particle p = { Random::randomVec3(minPos, maxPos), Random::random((float)ttl / 2.0, (float)ttl * 2.0)};
				p.initialLifetime = p.lifetime;

				particles.push_back(p);
			}
		}
		else // Once capacity has been reached, we respawn particles
		{
			printf("reached\n");
			int added = 0; // Keeps track of how many we have found and added
			for (int i = 0;  i < particles.size(); ++i)
			{
				if (particles[i].lifetime <= 0)
				{
					Particle p = { Random::randomVec3(minPos, maxPos), Random::random((float)ttl / 2.0, (float)ttl * 2.0) };
					p.initialLifetime = p.lifetime;
					particles[i] = p;
					++added; // Each time we add one from back from the dead, keep track of it
				}

				// In order to revive the dead particles at a seeminly constant rate
				// with the spawn rate, I need to make sure that we don't add any more per frame
				// than the spawn rate allows.

				// If we reached the spawn rate with the number of revived particles
				// then we break out of the loop to prevent excess searching / revivals
				if (added >= spawnRate)
				{
					break;
				}
			}
		}
	}

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

		textureSize = JsonUtils::vectorToVec2(config["textureSize"]);
		cellSize = JsonUtils::vectorToVec2(config["cellSize"]);

		numberOfFrames = (int)(textureSize.x / cellSize.x);

		if (config.find("textureSrc") != config.end())
		{
			setTexture(std::shared_ptr<Texture>(new Texture(config["textureSrc"])));
		}

		particles.reserve(maxParticles);
		positions = new float[maxParticles * 4]; // x, y, z, lifetime
		uvs = new float[maxParticles * 2 * 4]; // 2 coordinates (u, v) per vertex, and there are 4 vertices / quad

		sortedPositions = new float[maxParticles * 4]; // x, y, z, lifetime
		sortedUvs = new float[maxParticles * 2 * 4]; // 2 coordinates (u, v) per vertex, and there are 4 vertices / quad
		memset(positions, 0, maxParticles * 4);
		memset(uvs, 0, maxParticles * 2 * 4);
		memset(sortedPositions, 0, maxParticles * 4);
		memset(sortedUvs, 0, maxParticles * 2 * 4);

		glGenBuffers(1, &vertexVBO);
		assert(vertexVBO >= 0);

		glGenBuffers(1, &positionVBO);
		assert(positionVBO >= 0);

		glGenBuffers(1, &textureVBO);
		assert(textureVBO >= 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float) * 4, positions, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float) * 2 * 4, positions, GL_DYNAMIC_DRAW);

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

	int size() const { return particles.size(); }

private:
	long maxParticles;
	long spawnRate;
	long ttl;
	long numberOfFrames;
	float frameTime;

	unsigned int vertexVBO, positionVBO, textureVBO;
	unsigned int vao;

	std::shared_ptr<Texture> texture;
	std::vector<Particle> particles;
	std::vector<float> vertices;

	float* positions, *sortedPositions;
	float* uvs, *sortedUvs;

private:
	nlohmann::json config;

	glm::vec3 minPos, maxPos;
	glm::vec3 minScale, maxScale;
	glm::vec3 minRot, maxRot;
	glm::vec2 textureSize;
	glm::vec2 cellSize;
};