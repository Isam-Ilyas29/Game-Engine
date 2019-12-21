#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>

#include <iostream>

void generateVAO(unsigned int VAO) {
	//Binds the Vertex Array Object
	glBindVertexArray(VAO);
}

void generateVBO(float vertices[], unsigned int VBO) {
	//Bind and sets Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void generateEBO(unsigned int indices[], unsigned int EBO) {
	//Bind EBO, it stores indices that OpenGL uses to decide what vertices to draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);										 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

#endif
