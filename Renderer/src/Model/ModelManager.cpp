#include "ModelManager.h"

ModelManager::ModelManager(unsigned int vao)
: vao(vao)
{}

ModelManager::~ModelManager()
{}

void ModelManager::add(std::shared_ptr<Model> model)
{
	models.push_back(model);
}

void ModelManager::render(Shader& shader)
{
	for (auto model : models)
	{
		model->populateBuffers();
		shader.setUniformMat4(model->getModelMatrix(), "model");
		model->render(vao);
	}
}