#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Model.h"
#include "../Buffers/VertexBuffer.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

// TODO:
//	Add support for detecting and correcting when vertices are in the wrong order
//		This will prevent the OBJ model from needing the culling direction changed
class ObjModel : public Model
{
public:
	ObjModel() {}
	ObjModel(const std::string& fileName, unsigned int& vao);
	void render(GLuint& vao) { defaultRender(vao); }

private:
	std::vector<std::string> split(const std::string& str, const std::string& delim);
};

