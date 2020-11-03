#include "tree.h"

Tree::Tree()
{
	this->treeList = 0;
}

void Tree::Draw_Tree()
{
	Draw_List();
}

void Tree::SetTreeList(Shader& shader, string objfile)
{
	this->treeList = GenTreeList(shader, objfile);
}

GLint Tree::GenTreeList(Shader &shader, string objfile)
{
	GLint lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);	// 指定编号为lid，编译模式为仅编译
	tree2 = new myObjLoader(objfile, 3);
	tree2->Draw();

	glEndList();

	return lid;
}

void Tree::Draw_List()
{
	glPushMatrix();
	glCallList(this->treeList);
	glPopMatrix();
}

void Tree::Tree_Render(Shader& shader, Texture2D& texture)
{
	tree2->Render(shader, texture);
}