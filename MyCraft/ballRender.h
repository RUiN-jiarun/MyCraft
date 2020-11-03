#pragma once
#ifndef BALL_H
#define BALL_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "resource_manager.h"

using namespace std;

class Ball {
private:
	GLfloat radius;		// 半径
	GLint slices;		// 经线的条数
	GLint stacks;		// 纬线的条数

	vector<float> vertices;
	vector<float> normals;
	vector<float> texCoords;
	vector<unsigned int> indices;

	vector<float> interleavedVertices;
	int interleavedStride;

public:
	GLuint vao, vbo, ebo;

	Ball();

	unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
	unsigned int getNormalCount() const { return (unsigned int)normals.size() / 3; }
	unsigned int getTexCoordCount() const { return (unsigned int)texCoords.size() / 2; }
	unsigned int getIndexCount() const { return (unsigned int)indices.size(); }
	unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
	unsigned int getNormalSize() const { return (unsigned int)normals.size() * sizeof(float); }
	unsigned int getTexCoordSize() const { return (unsigned int)texCoords.size() * sizeof(float); }
	unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }

	void buildVertices();

	void buildInterleavedVertices();

	void Draw_Ball();

	void initRenderData();

	void Ball_Render(Shader& shader, Texture2D& texture);

};


#endif
