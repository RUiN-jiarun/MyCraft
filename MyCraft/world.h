#ifndef WORLD_H
#define WORLD_H

#pragma once

#include "GL/glew.h"
#include "cubeRender.h"
#include "skyBox.h"
#include "tree.h"
#include "ballRender.h"
#include "chunk/chunk.h"
#include "objreader.h"

#include <vector>
#include <unordered_map>

using namespace std;
class Camera;

class World
{
private:
	CubeRender* cubeRender;
	Tree* treeRender;
	Ball* ballRender;

	bool picked = false;
	glm::vec3 pickedBlock;

	SkyBox skyBox;
	unordered_map <int, Chunk*> chunks;
	Shader sence, chunkshader;

	Texture2DArray textureArray;

	Texture2D trunks;
	Texture2D leaves;
	Texture2D bird;
	Texture2D sun;

	// 坐标区块化
	int getChunk(int x);

	// 确定指定二维坐标处是否存在区块
	Chunk* findChunk(int x, int z);

	//世界物体材质
	float mAmbientMaterial[4], mDiffuseMaterial[4], mSpecularMaterial[4];

public:
	World();
	~World();
	bool isDay = true;
	

	GLfloat lightness_level = 0.45f;

	// 修改需要绘制的线框位置
	void pick_block(int x, int y, int z);
	void unpick_block();

	// 获取指定世界坐标中的方块类型
	BlockType get_block(int x, int y, int z);

	// 在世界坐标中放置指定方块
	void put_block(int x, int y, int z, BlockType type);

	// 移除指定坐标的方块
	void remove_block(int x, int y, int z);

	// 获取指定二维坐标出的海拔
	int highest(int x, int z);

	// 初始化世界生成，准备着色器，纹理等
	void init();

	// 渲染整个世界
	void render(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos);

};



#endif