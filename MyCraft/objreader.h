#ifndef OBJREADER_H
#define OBJREADER_H

#pragma once 
#include <GL/glew.h>
#include <vector>
#include <string>
#include "resource_manager.h"

using namespace std;

class Vertex3f {
public:
	float x, y, z;
};

class Vertex2f {
public:
	float x, y;
};

class Face {
public:
	int v[4];
	int vt[4];
	int vn[4];
};

class myObjLoader {
public:
	myObjLoader(string objfilename, int t);
	//void loadMtl(string mtlfilename);
	void Draw();
	void Render(Shader& shader, Texture2D& texture);
	GLuint vao, vbo;
	GLuint ibo;			// 顶点索引数组
	GLuint vnum;

private:
	vector<Vertex3f> Vertices;
	vector<Vertex2f> tVertices;
	vector<Vertex3f> Normals;
	vector<Face> Faces;

	vector<GLuint> Indices;
	vector<GLuint> TexIndices;
	GLuint texture;


	int facetype;

	
};


#endif