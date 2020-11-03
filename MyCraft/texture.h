#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once
#include "GL/glew.h"
#include <vector>
#include "GL/glut.h"

#define GL_CLAMP_TO_EDGE    0x812F


// 纹理加载、生成类
class Texture2D
{
private:

	GLuint Width, Height; 

public:
	// 为每一个材质生成一个ID
	GLuint ID;

	GLuint Internal_Format;
	GLuint Image_Format;

	Texture2D();

	// 生成纹理
	void Generate(GLuint width, GLuint height, unsigned char* data);

	// 绑定纹理
	void Bind() const;
};

// 纹理数组加载、生成
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