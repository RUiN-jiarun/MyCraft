#include "cubeRender.h"
#include "resource_manager.h"
#include <iostream>

CubeRender::CubeRender(Shader& lineShader)
{
	// 着色器通过资源管理类的接口调用
	// lineShader：绘制边框
	this->lineShader = lineShader;
	this->initRenderData();
}

void CubeRender::initRenderData()
{
	const float vertices[][3] = {
		-1, -1, -1,
		1, -1, -1,
		1,  1, -1,
		-1,  1, -1,
		-1, -1,  1,
		1, -1,  1,
		1,  1,  1,
		-1,  1,  1,
	};

	const int indices[] = {
		0, 1, 0, 3, 0, 4,
		1, 2, 1, 5, 2, 3,
		2, 6, 3, 7, 4, 5,
		4, 7, 5, 6, 6, 7
	};

	float data[24*3];
	for (int i = 0; i < 24; i++) {
		data[i*3] = vertices[indices[i]][0]*0.6;
		data[i*3+1] = vertices[indices[i]][1]*0.6;
		data[i*3+2] = vertices[indices[i]][2]*0.6;
	}

	// 生成并绑定vao与vbo
	glGenVertexArrays(1, &wireCubeVao);
	glBindVertexArray(wireCubeVao);
	glGenBuffers(1, &wireCubeVbo);
	glBindBuffer(GL_ARRAY_BUFFER, wireCubeVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void *)0);
	glDrawArrays(GL_LINES, 0, 24);
	glBindVertexArray(0);
}

void CubeRender::drawWireCube(glm::mat4 matrix)
{
	lineShader.Use();
	lineShader.SetMatrix4("matrix", matrix, false);

	glBindVertexArray(wireCubeVao);
	glDrawArrays(GL_LINES, 0, 24);
	glBindVertexArray(0);

	// 注意这里要还原成 GL_FILL, 保证多边形绘制模式是 GL_FILL
	// 否则天空盒会有问题, 待解决
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}