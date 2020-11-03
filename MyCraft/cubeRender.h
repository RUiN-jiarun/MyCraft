#ifndef CUBE_H
#define CUBE_H

#pragma once

#include "GL/glew.h"
#include "texture.h"
#include "shader.h"
#include "blockType.h"
#include <string>
#include <map>

// 方块渲染器
class CubeRender
{
private:
	// 线形着色器——方块边框
	Shader lineShader;

	// 方块线框的VAO和VBO
	GLuint wireCubeVao, wireCubeVbo;

public:
	// 渲染方块
	CubeRender(Shader& lineShader);

	// 配置VAO与VBO 
	void initRenderData();

	// 渲染边框
	void drawWireCube(glm::mat4 matrix);
};

#endif