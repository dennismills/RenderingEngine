#pragma once
#include "../Model/Model.h"

class Particle : public Model
{
public:
	Particle()
	{

		// TL, BL, BR      BR, TR, TL
		addOBJVertex({ glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0, 0.0) });
		addOBJVertex({ glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0, 1.0) });
		addOBJVertex({ glm::vec3(1.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0, 1.0) });

		addOBJVertex({ glm::vec3(1.0, -1.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0, 1.0) });
		addOBJVertex({ glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(1.0, 0.0) });
		addOBJVertex({ glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0, 0.0) });
	}

	virtual void update()
	{

	}


private:
	float timeToTill;
	glm::vec3 scale;
	glm::vec3 rot;
	glm::vec3 pos;
};

