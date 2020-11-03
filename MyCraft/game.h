#pragma once
#include "GL/glew.h"
#include "camera.h"
#include "world.h"
#include "mousePicker.h"
#include "objwriter.h"

// 游戏状态
enum GameState {
	GAME_ACTIVE,
	GAME_MENU
};


class Game
{
private:
	BlockType currentType;	// 当前鼠标要放置的方块类型

public:
	GameState  State;
	GLboolean  Keys[1024];
	
	// 游戏窗口的宽高
	GLuint     Width, Height;

	// 视角类
	Camera* camera;

	// 鼠标检测类
	MousePicker* mousePicker;

	// 世界渲染
	World* world;

	void nextBlcokType();

	Game(GLuint width, GLuint height);
	~Game();
	
	// 加载资源(着色器, 纹理), 初始化游戏状态
	void Init();

	// 使用存储在 keys 数组中的数据来处理用户输入
	void ProcessInput(GLfloat dt);

	// 鼠标移动操作——调用mousepicker类
	void MouseMoveCallback(int x, int y);

	void MouseClickCallback(int button, int state, int x, int y);


	// 窗口改变时更新摄像机状态
	void Reshape(int w, int h);

	// 在该函数中更新游戏状态设置,例如动物的自己移动
	void Update(GLfloat dt);

	// 渲染游戏
	void Render();
	
    myObjwriter* Objwriter;

private:

	void render_cross();



};