#pragma once
#include <cassert>
#include <string>
#include <gl/glew.h>

#include "../stb_image/stb_image.h"

class Texture
{
public:
	Texture() {}
	Texture(const std::string& fileName);
	Texture(const Texture& texture);
	~Texture() { printf("Called \n"); }

	Texture operator=(Texture rhs);

	void bind();

	const unsigned char* getPixels() const { return pixels; }
	const int getWidth() const { return width; }
	const int getHeight() const { return height; }
	const int getChannels() const { return channels; }
	GLuint getID() const { return textureID; }

private:
	GLuint textureID;
	int width, height, channels;
	unsigned char* pixels;
};
