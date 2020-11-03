#ifndef SKYBOX_H
#define SKYBOX_H

#pragma once

#include "shader.h"
#include "GL/SOIL.h"

#include <vector>
#include <iostream>

// 天空盒类——使用cubemap纹理
class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	
	void render();
	void init(Shader sky_shader);

private:
	Shader sky_shader;
	GLuint VBO;
	GLuint VAO;
	GLuint cubemapTexture;
	GLuint loadCubemap(std::vector<const char*> faces);
};

#endif