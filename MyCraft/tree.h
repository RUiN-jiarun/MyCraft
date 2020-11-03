#ifndef TREE_H
#define TREE_H

#pragma once

#include "objreader.h"
#include "resource_manager.h"

// 实际上用Tree类来渲染所有的obj物体，包括了鸟。
class Tree {
public:
	Tree();
	void Draw_Tree();
	void SetTreeList(Shader& shader, string objfile);
	void Tree_Render(Shader& shader, Texture2D& texture);

private:
	GLint treeList;
	myObjLoader* tree2;

	GLint GenTreeList(Shader& shader, string objfile);
	void Draw_List();

};


#endif