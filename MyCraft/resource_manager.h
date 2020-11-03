#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "texture.h"
#include "shader.h"

using namespace std;


// 资源管理类
// 所有的纹理和着色器资源都使用该类添加
// 该类会从文件中读取着色器程序或资源文件，然后存储起来
// 通过字符串名字来获取相应的资源

class ResourceManager
{
public:
	// 资源储存结构
	static map<std::string, Shader> Shaders;
	static map<std::string, Texture2D> Textures;
	static map<std::string, Texture2DArray> TextureArrays;

	// 加载着色器（接口）
	static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	
	// 获取着色器
	static Shader GetShader(string name);
	
	// 加载纹理（接口）
	static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, string name);
	
	// 获取纹理
	static Texture2D GetTexture(string name);

	// 纹理数组的载入和获取（主要是方块的六面纹理）
	static Texture2DArray LoadTextureArray(vector<const GLchar*> files, 
		GLboolean alpha, string name);
	static Texture2DArray GetTextureArray(string name);
	
	// 清除
	static void Clear();

protected:
	// 保护构造函数，我们不可创建该类的实例
	ResourceManager() { }
	
private:
	// 从文件载入着色器
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	
	// 从文件载入纹理
	static Texture2D loadTextureFromFile(const GLchar* file, GLboolean alpha);

	// 从文件载入多个纹理到纹理数组中
	static Texture2DArray loadTextureArrayFromFile(vector<const GLchar*> files, GLboolean alpha);
};

#endif