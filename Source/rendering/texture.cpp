#include "texture.h"


Texture::Texture(unsigned char* data, int width, int height, int channels, GLuint components)
	: mData(data), mWidth(width), mHeight(height), mChannels(channels), mFormat(0){

	stbi_set_flip_vertically_on_load(true);

	switch (components) {
	case 1:
		mFormat = GL_RED;
		break;
	case 2:
		mFormat = GL_RG;
		break;
	case 3:
		mFormat = GL_RGB;
		break;
	case 4:
		mFormat = GL_RGBA;
		break;
	default:
		std::cerr << "ERROR::TEXTURE::TYPE_NOT_SPECIFIED" << std::endl;
		break;
	}

	mID = dataToTextureID(mData, mWidth, mHeight, mFormat);

	stbi_image_free(data);
}

Texture::~Texture(){
	glDeleteTextures(1, &mID);
}

GLuint Texture::dataToTextureID(unsigned char* data, int width, int height, GLuint type){
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Loads image | Creates texture | Generates mipmaps
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load texture" << std::endl;
	}

	return textureID;
}

GLuint Texture::getID() const {
	return mID;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, getID());
}

GLuint Texture::getType() const {
	return mFormat;
}

int Texture::getWidth() const {
	return mHeight;
}

int Texture::getHeight() const {
	return mWidth;
}


