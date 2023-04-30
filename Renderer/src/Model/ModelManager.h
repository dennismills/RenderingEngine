#pragma once
#include "../Model/Model.h"
#include "../Shader/Shader.h"

#include <vector>
#include <algorithm>

class ModelManager
{
public:
	ModelManager() {}
	ModelManager(unsigned int vao);
	~ModelManager();
	void add(std::shared_ptr<Model> model);
	void render(Shader& shader);

	const unsigned int size() const { return (unsigned int)models.size(); }
	Model& operator[](unsigned int index) { return *models[index]; }

private:
	std::vector<std::shared_ptr<Model>> models;
	unsigned int vao;
};

