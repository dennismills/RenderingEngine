#include <GL/glew.h>
#include <iostream>

#pragma once
class VertexBuffer
{
public:
	VertexBuffer() {}
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	void bind();
	void unbind();
	void free();

private:
	unsigned int id;
};

