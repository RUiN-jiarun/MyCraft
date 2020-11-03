#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once
#include "GL/glew.h"
#include <vector>
#include "GL/glut.h"

#define GL_CLAMP_TO_EDGE    0x812F


// ������ء�������
class Texture2D
{
private:

	GLuint Width, Height; 

public:
	// Ϊÿһ����������һ��ID
	GLuint ID;

	GLuint Internal_Format;
	GLuint Image_Format;

	Texture2D();

	// ��������
	void Generate(GLuint width, GLuint height, unsigned char* data);

	// ������
	void Bind() const;
};

// ����������ء�����
class Texture2DArray
{
private:

	GLuint Width, Height; 

public:

	GLuint ID;

	GLuint Internal_Format;
	GLuint Image_Format;

	Texture2DArray();

	void Generate(GLuint width, GLuint height, std::vector<unsigned char*> datas);

	void Bind() const;

};

#endif