#include "ModelManager.h"

ModelManager::ModelManager(unsigned int vao)
: vao(vao)
{}

ModelManager::~ModelManager()
{
	for (auto& model : models)
	{
		delete model;
	}
}

void ModelManager::add(Model* model)
{
	models.push_back(model);
}

void ModelManager::render(Shader& shader)
{
	for (Model* model : models)
	{
		model->populateBuffers();
		shader.setUniformMat4(model->getModelMatrix(), "model");
		model->render(vao);
	}
}