#include "Texture.h"

Texture::Texture(const std::string& fileName)
{
	width = 0;
	height = 0;
	pixels = nullptr;

	glGenTextures(1, &textureID);
	if (textureID < 0)
	{
		printf("Cannot create texture for image: %s\n", fileName.c_str());
		assert(false);
	}
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_uc* data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		pixels = nullptr;
		printf("Cannot load texture: %s\n", fileName.c_str());
		assert(false);
	}
	pixels = std::shared_ptr<unsigned char[]>(new unsigned char[width * height * channels]);
	std::memcpy(pixels.get(), data, width * height * channels);
	stbi_image_free(data);

}

Texture::Texture(const Texture& texture)
{
	this->textureID = texture.getID();
	this->width = texture.getWidth();
	this->height = texture.getHeight();
	this->channels = texture.getChannels();
	assert(width > 0 && height > 0);

	this->pixels = std::shared_ptr<unsigned char[]>(new unsigned char[width * height * channels]);
	if (texture.getPixels() != nullptr)
	{
		memcpy(this->pixels.get(), texture.getPixels().get(), (width * height * channels) * sizeof(unsigned char));
	}
	else
	{
		memset(this->pixels.get(), 0, width * height * channels * sizeof(unsigned char));
	}
}

Texture Texture::operator=(Texture rhs)
{
	this->textureID = rhs.getID();
	this->width = rhs.getWidth();
	this->height = rhs.getHeight();
	this->channels = rhs.getChannels();
	this->pixels = std::shared_ptr<unsigned char[]>(new unsigned char[width * height * channels]);
	memcpy(this->pixels.get(), rhs.getPixels().get(), (width * height * channels) * sizeof(unsigned char));
	return *this;
}

void Texture::bind()
{
	if (pixels != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		switch (channels)
		{
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels.get());
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.get());
			break;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Cannot bind texture\n");
	}
}