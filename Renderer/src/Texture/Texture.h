#pragma once
#include <cassert>
#include <string>
#include <memory>
#include <gl/glew.h>

#include "../stb_image/stb_image.h"

class Texture
{
public:
	Texture() {}
	Texture(const std::string& fileName);
	Texture(const Texture& texture);
	~Texture() { }

	Texture operator=(Texture rhs);

	void bind();

	const std::shared_ptr<unsigned char[]> getPixels() const { return pixels; }
	const int getWidth() const { return width; }
	const int getHeight() const { return height; }
	const int getChannels() const { return channels; }
	GLuint getID() const { return textureID; }

	static unsigned int load(std::string fileName)
	{
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
            {
                format = GL_RED;
            }
            else if (nrChannels == 3)
            {
                format = GL_RGB;
            }
            else if (nrChannels == 4)
            {
                format = GL_RGBA;
            }
            else
            {
                format = GL_RGB;
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
        }
        else
        {
            printf("Cannot load texture: %s\n", fileName.c_str());
            assert(false);
            stbi_image_free(data);
        }

        return textureID;
	}

private:
	GLuint textureID;
	int width, height, channels;
	std::shared_ptr<unsigned char[]> pixels;
};

