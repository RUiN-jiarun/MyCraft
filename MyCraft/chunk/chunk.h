#ifndef CHUNK_H
#define CHUNK_H

#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "../blockType.h"
#include "perlinNoise.h"

const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 128;

// 区块类，固有参数为x和z坐标，y高度由柏林噪声生成
class Chunk
{
public:
	int X, Z;			// 区块位置
	GLuint vao = 0;		// 顶点数组对象
	int vertsNum = 0;

	Chunk(int x, int z) ;
	~Chunk();

	void genChunk();	// 生成或载入区块
	void genBuffer();	// 生成并绑定buffer
	void render();		// 渲染区块

	int highest(int x, int z);	// 获取指定位置的最大高度

	BlockType getBlock(int x, int y, int z);			// 获取指定位置方块
	void putBlock(int x, int y, int z, BlockType w);	// 在指定位置放置方块
	void removeBlock(int x, int y, int z);				// 移除指定位置的方块

	bool isLoaded() const;	
	bool isDirty() const;
	
	static bool isBlock(BlockType type);		// 判断是否为实体方块

private:
	bool loaded = false;				// 用来判断方块是否已生成或载入
	bool dirty = true;					// 用来判断buffer是否已生成并绑定

	GLuint vbo = 0;						// 顶点缓冲对象
	std::unordered_map<int, BlockType> blocks;

	// 生成方块的缓冲区
	void genCubeBuffer(std::vector<float> &data, int x, int y, int z, BlockType w, 
		bool left, bool right, bool top, bool bottom, bool front, bool back);
};

#endif