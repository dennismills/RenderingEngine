#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{

}

VertexBuffer::~VertexBuffer()
{}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::free()
{
	glDeleteBuffers(1, &id);
}