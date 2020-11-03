#include "texture.h"
#include "GL/SOIL.h"
#include <iostream>
#include <stdlib.h>


Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB)
{
	// Éú³ÉID
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{

	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

Texture2DArray::Texture2DArray()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB)
{
	glGenTextures(1, &this->ID);
}

void Texture2DArray::Generate(GLuint width, GLuint height, std::vector<unsigned char*> datas)
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, width, height,	datas.size(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	for (int i = 0; i < datas.size(); i++) 
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, datas[i]);
	
}

void Texture2DArray::Bind() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
}


