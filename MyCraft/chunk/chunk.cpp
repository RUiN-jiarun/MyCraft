﻿#include "chunk.h"

Chunk::Chunk(int x, int z)
{
	X = x;
	Z = z;
}

Chunk::~Chunk()
{
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);
	if (vbo != 0)
		glDeleteBuffers(1, &vbo);
}

// 生成或载入区块
void Chunk::genChunk()
{
	PerlinNoise noise;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{ 
		for (int z = 0; z < CHUNK_SIZE; z++) 
		{
			// 方块的实际位置
			int xi = X * CHUNK_SIZE + x;
			int zi = Z * CHUNK_SIZE + z;

			// 根据实际位置计算最大高度
			double f = noise.octaveNoise(xi * 0.01, zi * 0.01, 4, 0.5);
			double g = noise.octaveNoise(-xi * 0.01, -zi * 0.01, 2, 0.9);
			int mh = g * 32 + 16;
			int h = f * mh;
			int t = 12;
			if (h <= t)
				h = t;

			// 以草方块填充
			for (int y = 0; y <= h; y++) 
			{
				int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
				blocks.insert(std::make_pair(key, GRASS));
			}
		}
	}

	// 标记为已生成
	loaded = true;
}

// 生成并绑定buffer
void Chunk::genBuffer()
{
	if (!loaded)
		return;

	std::vector<float> data;

	// 遍历所有方块
	for (auto it = blocks.begin(); it != blocks.end(); it++) {
		if (it->second == AIR) 
			continue;

		// 得到坐标
		int key = it->first;
		int z = key % CHUNK_SIZE;
		key /= CHUNK_SIZE;
		int x = key % CHUNK_SIZE;
		int y = key / CHUNK_SIZE;

		bool left, right, top, bottom, front, back;
		left = right = top = bottom = front = back = false;
		if (!isBlock(getBlock(x - 1, y, z))) left = true;
		if (!isBlock(getBlock(x + 1, y, z))) right = true;
		if (!isBlock(getBlock(x, y + 1, z))) top = true;
		if (!isBlock(getBlock(x, y - 1, z))) bottom = true;
		if (!isBlock(getBlock(x, y, z + 1))) front = true;
		if (!isBlock(getBlock(x, y, z - 1))) back = true;

		if (isBlock(it->second)) {		// 生成一个方块的buffer
			genCubeBuffer(data, x + X * CHUNK_SIZE, y, z + Z * CHUNK_SIZE, it->second,
				left, right, top, bottom, front, back);
		}
	}

	// 顶点数
	vertsNum = 0;
	if (data.size() != 0)
		vertsNum = data.size() / 12;

	// 删除已有的vao和vbo
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);
	if (vbo != 0)
		glDeleteBuffers(1, &vbo);

	// 生成并绑定vao，vbo
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// 将准备好的顶点数据复制到缓冲的内存中，用static格式表示基本不会改变数据，缓冲区相对静态
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	// 指定顶点的解析方式
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(9 * sizeof(float)));

	// 标记为已绑定buffer
	dirty = false;
}

// 渲染区块，调用VAO
void Chunk::render()
{
	if (!loaded)
		genChunk();
	if (dirty)
		genBuffer();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertsNum);
}

// 获取指定位置的最大高度
int Chunk::highest(int x, int z)
{
	int highest = 0;

	for (int y = 0; y < CHUNK_HEIGHT; y++) 
	{
		if (getBlock(x, y, z) == AIR)
			return y > 1 ? y - 1 : 0;
	}
	return CHUNK_HEIGHT;
}

// 获取指定位置的方块
BlockType Chunk::getBlock(int x, int y, int z)
{
	// 判断是否越界
	if (x > CHUNK_SIZE - 1 || y > CHUNK_HEIGHT - 1 || z > CHUNK_SIZE - 1)
		return AIR;
	if (x < 0 || y < 0 || z < 0)
		return AIR;

	int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
	auto it = blocks.find(key);
	if (it != blocks.end())
		return it->second;
	else
		return AIR;
}

// 在指定位置放置方块
void Chunk::putBlock(int x, int y, int z, BlockType w)
{
	// 判断是否越界
	if (x > CHUNK_SIZE - 1 || y > CHUNK_HEIGHT - 1 || z > CHUNK_SIZE - 1)
		return;
	if (x < 0 || y < 0 || z < 0)
		return;

	int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
	auto it = blocks.find(key);
	if (it != blocks.end())
		it->second = w;
	else
		blocks.insert(std::make_pair(key, w));

	dirty = true;
}

// 移除指定位置的方块
void Chunk::removeBlock(int x, int y, int z)
{
	// 判断是否越界
	if (x > CHUNK_SIZE - 1 || y > CHUNK_HEIGHT - 1 || z > CHUNK_SIZE - 1)
		return;
	if (x < 0 || y < 0 || z < 0)
		return;

	int key = (y * CHUNK_SIZE + x) * CHUNK_SIZE + z;
	auto it = blocks.find(key);
	if (it != blocks.end())
		blocks.erase(it);

	dirty = true;
}

bool Chunk::isLoaded() const
{
	return loaded;
}

bool Chunk::isDirty() const
{
	return dirty;
}


// 判断是否为实体方块
bool Chunk::isBlock(BlockType type) 
{
	return type != AIR && type != TREE;
}

// 生成一个方块的顶点缓冲区，用来绑定vao
void Chunk::genCubeBuffer(std::vector<float>& data, int x, int y, int z, BlockType w, bool left, bool right, bool top, bool bottom, bool front, bool back)
{
	static const float position[6][6][5] = {
		// left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		// right
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

		// top
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

		// bottom
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		// front
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

		// back
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	};

	static const float normals[6][3] = {
		{ -1, 0, 0 },
		{ +1, 0, 0 },
		{ 0, +1, 0 },
		{ 0, -1, 0 },
		{ 0, 0, +1 },
		{ 0, 0, -1 }
	};

	static const float tangents[6][3] = {
		{  0, 0,  1 },
		{  0, 0, -1 },
		{  1, 0,  0 },
		{  1, 0,  0 },
		{  1, 0,  0 },
		{ -1, 0,  0 }
	};

	bool faces[] = { left, right, top, bottom, front, back };

	// 遍历方块的6个面
	for (int i = 0; i < 6; i++) 
	{
		if (!faces[i])
			continue;

		// 遍历面上的6个顶点
		for (int v = 0; v < 6; v++) 
		{
			// xyz
			data.push_back(x + position[i][v][0]);
			data.push_back(y + position[i][v][1]);
			data.push_back(z + position[i][v][2]);
			// 法线
			data.push_back(normals[i][0]);
			data.push_back(normals[i][1]);
			data.push_back(normals[i][2]);
			// uv
			data.push_back(position[i][v][3]);
			data.push_back(position[i][v][4]);
			// 纹理
			data.push_back(ETextureType::blockTextures[w][i]);
			// 切线
			data.push_back(tangents[i][0]);
			data.push_back(tangents[i][1]);
			data.push_back(tangents[i][2]);
		}
	}
}
