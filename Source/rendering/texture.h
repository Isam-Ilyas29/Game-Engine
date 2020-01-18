#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include "../environment/environment.h"

#include <iostream>

class Texture {
private:
	unsigned char* mData;
	int mWidth, mHeight, mChannels;
	GLuint mID;
	GLuint mFormat;

public:
	Texture(unsigned char* data, int width, int height, int channels, GLuint components);
	~Texture();

	static GLuint dataToTextureID(unsigned char* data, int width, int height, GLuint type);
	GLuint getID() const;
	void bind();
	GLuint getType() const;
	int getWidth() const;
	int getHeight() const;
};
