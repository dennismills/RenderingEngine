#pragma once
#include "../Model/Model.h"
#include "../Shader/Shader.h"

#include <vector>

class ModelManager
{
public:
	ModelManager() {}
	ModelManager(unsigned int vao);
	~ModelManager();
	void add(Model* model);
	void render(Shader& shader);

	const unsigned int size() const { return models.size(); }
	Model& operator[](unsigned int index) { return *models[index]; }

private:
	std::vector<Model*> models;
	unsigned int vao;
};

