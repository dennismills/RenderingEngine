#pragma once
#include "../Model/Model.h"
#include "../Timer/Timer.h"

class Particle : public Model
{
public:
	Particle()
	{
		startTime = 0;
		alive = false;
		hasStarted = false;
		timeToLive = 0;
	}

	Particle(long ttl)
	{
		// TL, BL, BR      BR, TR, TL
		addVertex({ glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0, 0.0) });
		addVertex({ glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0, 0.0) });
		addVertex({ glm::vec3(1.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0, 1.0) });

		addVertex({ glm::vec3(1.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0, 1.0) });
		addVertex({ glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0, 1.0) });
		addVertex({ glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0, 0.0) });

		timeToLive = ttl;
		hasTexture = true;
		alive = true;
		hasStarted = false;

		startTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	void render(GLuint& vao)
	{
		if (!hasStarted)
		{
			hasStarted = true;
		}

		defaultRender(vao);
	}
	void update(long currentTime)
	{
		if (currentTime - startTime >= timeToLive)
		{
			alive = false;
		}
	}
	void setAlive(bool status) { alive = status; }
	void setTTL(long ttl) { this->timeToLive = ttl; }
	long getTTL() const { return timeToLive; }
	bool isAlive() const { return alive; }

protected:
	long startTime;
	bool alive;
	bool hasStarted;
	long timeToLive;
};

