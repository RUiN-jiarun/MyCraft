#include "world.h"
#include "resource_manager.h"
#include "light.h"
#include <ctime>
#include <glm\gtx\string_cast.hpp>


glm::vec3 light(1.0, 0.8, 0.8);			//灯光的信息
glm::vec3 lightpos(20, 20, 20);			//灯光位置
glm::vec3 ambient, diffuse, specular;	//材质的信息
float shiness = 32.0f;					//灯光参数

vector<glm::vec3> treepos;

int frame_num = 0;
float bird_x = 0, bird_x2 = 0, bird_x3 = 0;

const int WORLD_HALF_SIZE = 5;
World::World()
{
}

int World::getChunk(int x)
{
	return floor(float(x) / CHUNK_SIZE);
}

Chunk* World::findChunk(int x, int z)
{
	int key = (x + WORLD_HALF_SIZE) * WORLD_HALF_SIZE * 2 + (z + WORLD_HALF_SIZE);
	auto it = chunks.find(key);
	if (it != chunks.end())
		return it->second;
	else
		return nullptr;
}

World::~World()
{
	// 释放渲染器
	if (cubeRender != nullptr)
		delete cubeRender;
	//释放树木渲染器
	if (treeRender != nullptr)
		delete[] treeRender;
	if (ballRender != nullptr)
		delete ballRender;
}

void World::pick_block(int x, int y, int z)
{
	picked = true;
	pickedBlock = { x, y, z };
}

void World::unpick_block()
{
	picked = false;
}

BlockType World::get_block(int x, int y, int z)
{
	int X = getChunk(x), Z = getChunk(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return AIR;

	return c->getBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE);
}

void World::put_block(int x, int y, int z, BlockType type)
{
	int X = getChunk(x), Z = getChunk(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return;

	c->putBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE, type);
}

void World::remove_block(int x, int y, int z)
{
	int X = getChunk(x), Z = getChunk(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return;

	c->removeBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE);
}

int World::highest(int x, int z)
{
	int X = getChunk(x), Z = getChunk(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return 0;

	return c->highest(x - X * CHUNK_SIZE, z - Z * CHUNK_SIZE);
}

void World::init()
{
	// 初始化立方体渲染器
	Shader lineShader = ResourceManager::GetShader("shader_line");
	cubeRender = new CubeRender(lineShader);

	treeRender = new Tree[4];
	ballRender = new Ball();

	// 初始化天空盒
	skyBox = SkyBox();
	skyBox.init(ResourceManager::GetShader("shader_skybox"));

    chunkshader = ResourceManager::GetShader("shader_chunk");
	sence = ResourceManager::GetShader("shader_sence");

	textureArray = ResourceManager::GetTextureArray("blocks");		// 调用方块纹理管理
	

	trunks = ResourceManager::GetTexture("trunk");
	leaves = ResourceManager::GetTexture("leaves");
	bird = ResourceManager::GetTexture("bird");
	sun = ResourceManager::GetTexture("sun");


	// 初始化区块
	for (int i = -WORLD_HALF_SIZE; i < WORLD_HALF_SIZE; i++) {
		for (int j = -WORLD_HALF_SIZE; j < WORLD_HALF_SIZE; j++) {
			Chunk* c = findChunk(i, j);
			// 如果指定位置不存在区块，则创建或载入一个区块
			if (!c) {
				c = new Chunk(i, j);
				c->genChunk();
				c->genBuffer();
				int key = (i + WORLD_HALF_SIZE) * WORLD_HALF_SIZE * 2 + (j + WORLD_HALF_SIZE);
				chunks.insert(make_pair(key, c));
			}
		}
	}

	// 添加世界边缘
	for (int x = -WORLD_HALF_SIZE * CHUNK_SIZE; x < WORLD_HALF_SIZE * CHUNK_SIZE; x++)
	{
		for (int y = 0; y < 45; y++)
		{
			put_block(x, y, WORLD_HALF_SIZE * (CHUNK_SIZE - 0.05), GRASS);
			put_block(x, y, -WORLD_HALF_SIZE * CHUNK_SIZE, GRASS);
		}
	}

	for (int z = -WORLD_HALF_SIZE * CHUNK_SIZE; z < WORLD_HALF_SIZE * CHUNK_SIZE; z++)
	{
		for (int y = 0; y < 45; y++)
		{
			put_block(WORLD_HALF_SIZE * (CHUNK_SIZE - 0.05), y, z, GRASS);
			put_block(-WORLD_HALF_SIZE * CHUNK_SIZE, y, z, GRASS);
		}
	}



	//初始化树
	//树木位置
	int mapSize = WORLD_HALF_SIZE * 2 * CHUNK_SIZE;
	int treeNum = (WORLD_HALF_SIZE * 2) * (WORLD_HALF_SIZE * 2);
	srand((unsigned int)time(0));//初始化种子为随机值
	for (int i = 0; i < treeNum; i++) 
	{
		int x = rand() % mapSize - mapSize / 2;
		int z = rand() % mapSize - mapSize / 2;
		int y = highest(x, z);

		
		if (y < 40) 
		{
			if (i < 50)
				// 碰撞体积为1个方块
				put_block(x, y + 1, z, TREE);
			else
			{
				// 碰撞体积为3个方块
				put_block(x, y + 1, z, TREE);
				put_block(x, y + 2, z, TREE);
				put_block(x, y + 3, z, TREE);
			}
			// 树的位置
			treepos.push_back({x, y + 0.5, z});
		}		
	}
}


float ra = 0;
void World::render(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos)
{
	textureArray.Bind();
	if (isDay)
	{
		// 渲染天空盒
		skyBox.render();	
		// 光照移动
		lightpos.x = 60 * sin(glm::radians(ra));
		lightpos.y = 60;
		lightpos.z = 60 * cos(glm::radians(ra));
		ra += 0.10;
		if (ra > 360)
			ra -= 360;

		// 渲染区块
		glEnable(GL_CULL_FACE);
		chunkshader.Use();
		chunkshader.SetMatrix4("project", projection);
		chunkshader.SetMatrix4("view", view);
		chunkshader.SetMatrix4("model", glm::mat4(1.0));
		chunkshader.SetFloat("shiness", shiness);
		chunkshader.SetVector3f("lightPos", lightpos);
		chunkshader.SetVector3f("viewpos", cameraPos);
		chunkshader.SetVector3f("light.ambient", lightness_level, lightness_level, lightness_level);
		chunkshader.SetVector3f("light.diffuse", lightness_level + 0.15f, lightness_level + 0.15f, lightness_level + 0.15f);
		chunkshader.SetVector3f("light.specular", lightness_level + 0.25f, lightness_level + 0.25f, lightness_level + 0.25f);
		

		for (auto it : chunks) {
			it.second->render();
		}
		//渲染太阳
		glDisable(GL_CULL_FACE);
		sence.Use();
		sence.SetVector3f("lightcolor", light);
		sence.SetVector3f("lightpos", lightpos);
		sence.SetFloat("shiness", shiness);

		sence.SetVector3f("viewpos", cameraPos);
		sence.SetMatrix4("project", projection);

		ambient = glm::vec3(1, 0, 0);
		diffuse = glm::vec3(1, 0, 0);
		specular = glm::vec3(1, 1, 1);
		sence.SetVector3f("ambient", ambient);
		sence.SetVector3f("diffuse", diffuse);
		sence.SetVector3f("specular", specular);
		sence.SetMatrix4("view", view);
		sence.SetMatrix4("model", glm::translate(glm::mat4(1.0), lightpos));

		ballRender->Ball_Render(sence, sun);

	}
	else
	{
		// 渲染区块
		glEnable(GL_CULL_FACE);
		chunkshader.Use();
		chunkshader.SetMatrix4("project", projection);
		chunkshader.SetMatrix4("view", view);
		chunkshader.SetMatrix4("model", glm::mat4(1.0));
		chunkshader.SetFloat("shiness", shiness);
		chunkshader.SetVector3f("lightPos", lightpos);
		chunkshader.SetVector3f("viewpos", cameraPos);

		chunkshader.SetVector3f("light.ambient", 0.2f, 0.2f, 0.2f);
		chunkshader.SetVector3f("light.diffuse", 0.4f, 0.4f, 0.4f);
		chunkshader.SetVector3f("light.specular", 0.65f, 0.65f, 0.65f);

        for (auto it : chunks) {
		it.second->render();
	   }
	}

	

    glDisable(GL_CULL_FACE);
	
	
	// 渲染树木和鸟
	sence.Use();
	sence.SetVector3f("lightcolor", 0.4f, 0.4f, 0.4f);
	sence.SetVector3f("lightpos", lightpos);
	sence.SetFloat("shiness", shiness);
	sence.SetVector3f("viewpos", cameraPos);
	sence.SetMatrix4("project", projection);
	sence.SetMatrix4("view", view);
	

	string file_name = "obj/" + to_string(frame_num) + ".obj";
	treeRender[0].SetTreeList(sence, "obj/tree5.obj");
	treeRender[1].SetTreeList(sence, "obj/tree4.obj");
	treeRender[2].SetTreeList(sence, "obj/tree3.obj");
	treeRender[3].SetTreeList(sence, file_name);

	int bushcnt = 0;
	int bird_id = 0;
	int birdcnt = 0;
	bool drawbird = true;
	bool drawbush = true;
	for (auto pos : treepos)
	{
		//if (drawbird)
		{
			if ((bird_id % 3) == 0)
			{
				ambient = glm::vec3(1, 1, 1);
				diffuse = glm::vec3(0.5, 0.8, 0);
				specular = glm::vec3(0.3, 0.8, 0);
				sence.SetVector3f("ambient", ambient);
				sence.SetVector3f("diffuse", diffuse);
				sence.SetVector3f("specular", specular);
				glm::mat4 model_ani = glm::mat4(1.0);
				model_ani = glm::translate(model_ani, glm::vec3(bird_x + pos.x, 50 + pos.y, 0 + pos.z));
				sence.SetMatrix4("model", model_ani);
				treeRender[3].Tree_Render(sence, bird);
			}
			else if ((bird_id % 3) == 1)
			{
				ambient = glm::vec3(1, 1, 1);
				diffuse = glm::vec3(0.5, 0.8, 0);
				specular = glm::vec3(0.3, 0.8, 0);
				sence.SetVector3f("ambient", ambient);
				sence.SetVector3f("diffuse", diffuse);
				sence.SetVector3f("specular", specular);
				glm::mat4 model_ani = glm::mat4(1.0);
				model_ani = glm::translate(model_ani, glm::vec3(bird_x2 + pos.x, 50 + pos.y, 0 + pos.z));
				sence.SetMatrix4("model", model_ani);
				treeRender[3].Tree_Render(sence, bird);
			}
			else if ((bird_id % 3) == 2)
			{
				ambient = glm::vec3(1, 1, 1);
				diffuse = glm::vec3(0.5, 0.8, 0);
				specular = glm::vec3(0.3, 0.8, 0);
				sence.SetVector3f("ambient", ambient);
				sence.SetVector3f("diffuse", diffuse);
				sence.SetVector3f("specular", specular);
				glm::mat4 model_ani = glm::mat4(1.0);
				model_ani = glm::translate(model_ani, glm::vec3(bird_x3 + pos.x, 50 + pos.y, 0 + pos.z));
				sence.SetMatrix4("model", model_ani);
				treeRender[3].Tree_Render(sence, bird);
			}
			bird_id++;
			birdcnt++;
		}
		//if (birdcnt == 50)
			//drawbird = false;

		if (drawbush)
		{
			ambient = glm::vec3(0, 1, 0);
			diffuse = glm::vec3(0.5, 0.8, 0);
			specular = glm::vec3(0.3, 0.8, 0);
			sence.SetVector3f("ambient", ambient);
			sence.SetVector3f("diffuse", diffuse);
			sence.SetVector3f("specular", specular);
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
			sence.SetMatrix4("model", model);
			treeRender[0].Tree_Render(sence, leaves);
			bushcnt += 1;
		}
		if (bushcnt == 50)
			drawbush = false;
		if (!drawbush)
		{
			ambient = glm::vec3(0.8, 0.3, 0.3);
			diffuse = glm::vec3(0.6, 0.2, 0.5);
			specular = glm::vec3(0.5, 0, 0);
			sence.SetVector3f("ambient", ambient);
			sence.SetVector3f("diffuse", diffuse);
			sence.SetVector3f("specular", specular);
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
			sence.SetMatrix4("model", model);
			treeRender[1].Tree_Render(sence, trunks);

			ambient = glm::vec3(0, 1, 0);
			diffuse = glm::vec3(0.1, 0.8, 0);
			specular = glm::vec3(0.3, 0.8, 0);
			sence.SetVector3f("ambient", ambient);
			sence.SetVector3f("diffuse", diffuse);
			sence.SetVector3f("specular", specular);

			treeRender[2].Tree_Render(sence, leaves);
		}
	}


	// 渲染方块边框
	if (picked) 
	{
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, {pickedBlock.x, pickedBlock.y, pickedBlock.z});
		glm::mat4 matrix = projection * view * model;
		cubeRender->drawWireCube(matrix);
	}

	glUseProgram(0);
}

