#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/SOIL.h>

// 实例化静态成员变量
map<string, Texture2D> ResourceManager::Textures;
map<string, Shader> ResourceManager::Shaders;
map<string, Texture2DArray> ResourceManager::TextureArrays;


Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(string name)
{
	return Textures[name];
}

Texture2DArray ResourceManager::LoadTextureArray(std::vector<const GLchar*> files, GLboolean alpha, string name)
{
	TextureArrays[name] = loadTextureArrayFromFile(files, alpha);
	return TextureArrays[name];
}

Texture2DArray ResourceManager::GetTextureArray(string name)
{
	return TextureArrays[name];
}

void ResourceManager::Clear()
{	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
	for (auto iter : TextureArrays)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile)
{
	string vertexCode;
	string fragmentCode;
	string geometryCode;
	try
	{
		// 打开文件
		ifstream vertexShaderFile(vShaderFile);
		ifstream fragmentShaderFile(fShaderFile);
		stringstream vShaderStream, fShaderStream;
		// 读取文件缓冲内容到数据流
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// 关闭文件处理器
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// 转换数据流到字符串
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderFile != nullptr)
		{
			ifstream geometryShaderFile(gShaderFile);
			stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		// 读取异常抛出错误
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();

	// 从源代码创建着色器对象并编译
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar* file, GLboolean alpha)
{
	// 创建纹理对象
	Texture2D texture;
	if (alpha)
	{
		// 格式为RGBA
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// 读取图像
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// 生成纹理
	texture.Generate(width, height, image);
	// 释放数据
	SOIL_free_image_data(image);
	return texture;
}

Texture2DArray ResourceManager::loadTextureArrayFromFile(vector<const GLchar*> files, GLboolean alpha)
{
	Texture2DArray textureArray;
	if (alpha)
	{
		textureArray.Internal_Format = GL_RGBA;
		textureArray.Image_Format = GL_RGBA;
	}
	int width, height;
	vector<unsigned char*> images;
	for (auto iter : files) {
		unsigned char* image = SOIL_load_image(iter, &width, &height, 0, 
			textureArray.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		images.push_back(image);
	}
	textureArray.Generate(width, height, images);
	for (auto iter : images)
		SOIL_free_image_data(iter);
	return textureArray;
}
