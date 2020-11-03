#include "game.h"
#include "resource_manager.h"

Game::Game(GLuint width, GLuint height)
{
	this->Width = width;
	this->Height = height;

	camera = nullptr;

	world = nullptr;

	mousePicker = nullptr;

	currentType = GRASS;

	for (int i = 0; i < 1024; i++)
	{
		Keys[i] = false;
	}
	State = GAME_ACTIVE;

	Objwriter = new myObjwriter();
}

Game::~Game()
{
	if (camera != nullptr)
		delete camera;
	if (world != nullptr)
		delete world;
}

void Game::nextBlcokType()
{
	this->currentType = (BlockType)((this->currentType + 1) % BlockTypeNum);
	//cout << this->currentType << endl;
}


void Game::Init()
{
	// 纹理数组
	vector<const GLchar*> files;
	files.push_back("textures/grass_top.png");
	files.push_back("textures/grass_top_s.png");
	files.push_back("textures/grass_top_n.png");
	files.push_back("textures/grass_side.png");
	files.push_back("textures/grass_side_s.png");
	files.push_back("textures/grass_side_n.png");
	files.push_back("textures/ice.png");
	files.push_back("textures/ice_s.png");
	files.push_back("textures/ice_n.png");
	files.push_back("textures/gold_ore.png");
	files.push_back("textures/gold_ore_s.png");
	files.push_back("textures/gold_ore_n.png");
	files.push_back("textures/stone.png");
	files.push_back("textures/stone_s.png");
	files.push_back("textures/stone_n.png");
	files.push_back("textures/oak_planks.png");
	files.push_back("textures/oak_planks_s.png");
	files.push_back("textures/oak_planks_n.png");
	ResourceManager::LoadTextureArray(files, false, "blocks");

	ResourceManager::LoadTexture("textures/tree.png", false, "trunk");
	ResourceManager::LoadTexture("textures/leaf.png", false, "leaves");
	ResourceManager::LoadTexture("textures/bird.bmp", false, "bird");
	ResourceManager::LoadTexture("textures/sunmap.bmp", false, "sun");

	ResourceManager::LoadShader("shaders/sky_vertex.glsl", "shaders/sky_fragment.glsl", nullptr, "shader_skybox");
	ResourceManager::LoadShader("shaders/chunk_vertex.glsl", "shaders/chunk_fragment.glsl",
		nullptr, "shader_chunk");
	ResourceManager::LoadShader("shaders/line_vertex.glsl", "shaders/line_fragment.glsl",
		nullptr, "shader_line");
	ResourceManager::LoadShader("shaders/sence_vertex.glsl", "shaders/sence_fragment.glsl", nullptr, "shader_sence");

	// 新建一个 world 对象
	this->world = new World();
	// 初始化世界
	this->world->init();

	// 创建一个照相机
	this->camera = new Camera(glm::vec3(3.0f, this->world->highest(3, 3) + 2.5f, 3.0f));

	// 创建鼠标拾取器
	// this->mousePicker = new MousePicker(this->camera, glm::make_mat4(projectionMatrix));
	this->mousePicker = new MousePicker(this->camera, glm::perspective(65.0f, (float)Width / Height, 0.125f, 100.0f));

	currentType = GRASS;
}

void Game::ProcessInput(GLfloat dt)
{
	camera->doMovement(this->Keys, world, dt);
	camera->doZoom(this->Keys, dt);
}


void Game::Render()
{
	// 清除缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	// 设置投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera->getZoom(), (float)Width / Height, 0.125f, 100.0f);

	// 设置模型矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 设置视点
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), (float)Width / Height, 0.125f, 100.0f);
	//	glm::mat4 matrix = projection * view;

	// 渲染世界
	world->render(projection, view, camera->Position);

	// 渲染屏幕中心的十字
	this->render_cross();

}

void Game::Update(GLfloat dt)
{
}


static int block_last_x = 0;
static int block_last_y = 0;
static int block_last_z = 0;
static bool picked = false;

void Game::MouseMoveCallback(int xpos, int ypos)
{
	this->camera->camera_mouse_callback(xpos, ypos);

	// 更新picker
	this->mousePicker->update(xpos, ypos, Width, Height);

	// 射线pick检测
	int distance = 8;
	bool isPicked = false;
	glm::vec3 ray = this->mousePicker->getCurrentRay();
	int x, y, z;
	for (int u = 0; u < distance; u++)
	{
		x = roundf(ray.x * u + camera->Position.x);
		y = roundf(ray.y * u + camera->Position.y);
		z = roundf(ray.z * u + camera->Position.z);
		if (Chunk::isBlock(world->get_block(x, y, z)))
		{
			if (block_last_x == x && block_last_y == y && block_last_z == z && picked)
			{
				// 已经选定，不做操作
			}
			else // 调用函数来求交
			{
				world->unpick_block();
				world->pick_block(x, y, z);
				block_last_x = x;
				block_last_y = y;
				block_last_z = z;
				picked = true;
			}
			isPicked = true;
			break;
		}
	}
	// 如果没有被选择的方块，则取消选择
	if (!isPicked)
	{
		world->unpick_block();
		picked = false;
	}
}

void Game::MouseClickCallback(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			// 放置方块
			glm::vec3 ray = this->mousePicker->getCurrentRay();
			glm::vec3 pos = glm::vec3(block_last_x, block_last_y, block_last_z);
			// 获取与鼠标射线相交的面
			glm::vec3 p = this->mousePicker->Intersect(ray, camera->Position, block_last_x, block_last_y, block_last_z);
			if (p == glm::vec3(0, 0, 0))
				return;
			pos += p;

			if (world->get_block(pos.x, pos.y, pos.z) == AIR)
			{	// 当前位置没有方块
				if (glm::abs(camera->Position.x - pos.x) < 0.6 && glm::abs(camera->Position.z - pos.z) < 0.6 && pos.y + 0.3 > camera->Position.y - 1.3 && pos.y - 0.3 < camera->Position.y + 0.3) 	// 要放的方块不能与人碰撞
					return;

				world->put_block(pos.x, pos.y, pos.z, currentType);
				// 加入obj导出的队列
				Objwriter->addCude(pos);
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON)
	{		// 消除方块
		if (state == GLUT_DOWN)
		{
			if (Chunk::isBlock(world->get_block(block_last_x, block_last_y, block_last_z)))
			{
				world->remove_block(block_last_x, block_last_y, block_last_z);
				
			}
			
		}
	}

}


void Game::Reshape(int w, int h)
{
	Width = w;
	Height = h;
	camera->setProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)w / h, 0.125f, 100.0f));
}

// 画鼠标中心
void Game::render_cross()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);		// 选择投影矩阵
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 100, 0, 100, -1, 1);		// 位置正投影
	glMatrixMode(GL_MODELVIEW);			// 选择Modelview矩阵
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glVertex2f(49, 50);
	glVertex2f(51, 50);
	glVertex2f(50, 48.5);
	glVertex2f(50, 51.5);
	glEnd();

	glMatrixMode(GL_PROJECTION);		// 选择投影矩阵
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);			// 选择Modelview矩阵
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}


