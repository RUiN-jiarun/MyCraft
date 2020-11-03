#pragma once
#ifndef OBJWRITER_H
#define OBJWRITER_H

#pragma once 
#include <GL/glew.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "resource_manager.h"
using namespace std;


class ObjFace {
public:
	int v[4];
	int vt[4];
	int vn[4];
	void change_value(int group, int v1,  int vn1, int v2, int vn2, int v3, int vn3)
	{
		v[0] = group * 8 + v1;
		v[1] = group * 8 + v2;
		v[2] = group * 8 + v3;
		vn[0] = group * 24 + vn1;
		vn[1] = group * 24 + vn2;
		vn[2] = group * 24 + vn3;
	}
};




class myObjwriter {
public:
	myObjwriter(string objfilename);
	myObjwriter(){}
	//void loadMtl(string mtlfilename);
	void Output(struct tm* ptminfo);
	void addCude(glm::vec3 new_vertice);
	/*
	GLuint vao, vbo;
	GLuint ibo;			// 顶点索引数组
	GLuint vnum;
	*/
private:
	vector<glm::vec3> Vertices;
	vector<glm::vec3> tVertices;
	vector<glm::vec3> Normals;
	vector<ObjFace> Faces;
	string file_name;

	glm::vec3 addvector(glm::vec3, int xx, int yy, int zz);

};


#endif