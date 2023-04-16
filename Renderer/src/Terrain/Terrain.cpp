#include "Terrain.h"

Terrain::Terrain(const unsigned int width, const unsigned int height, unsigned int& vao)
:	width(width),
	height(height)
{
    octaves = 1;
    persistance = 0.5f;
    float scale = 1.0f;
    populateBuffers(vao);
}