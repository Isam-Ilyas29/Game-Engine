#include "texture.h"


Texture::Texture(unsigned char* data, int width, int height, GLuint components) 
	: mData(data), mWidth(width), mHeight(height), mType(0), mID(0){
	
	switch (components) {
	case 1:
		mType = GL_RGB;
		break;
	case 2:
		mType = GL_RGBA;
		break;
	default:
		std::cerr << "ERROR::TEXTURE::TYPE_NOT_SPECIFIED" << std::endl;
	}
}

Texture::~Texture(){
	glDeleteTextures(1, &mID);
}

GLuint Texture::dataToTextureID(unsigned char* data, int width, int height, GLuint type){
	GLuint textureID;

	//Tells stb_image to flip loaded texture's on the y-axis.
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Sets the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Sets texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Loads image | Creates texture | Generates mipmaps
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	//If the texture fails to load
	else {
		//Texture loading error message [1]:
		std::cerr << "Failed to load texture_one" << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}

GLuint Texture::getID() {
	if (mID == 0) {
		mID = dataToTextureID(mData, mWidth, mHeight, mType);
	}

	return mID;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, getID());
}

GLuint Texture::getType() const {
	return mType;
}

int Texture::getWidth() const {
	return mHeight;
}

int Texture::getHeight() const {
	return mWidth;
}


