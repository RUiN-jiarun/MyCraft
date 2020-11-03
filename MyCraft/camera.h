#ifndef CAMERA_H
#define CAMERA_H

#pragma once
#include <vector>
#include <iostream>


#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "world.h"
#include <vector>

using namespace std;

class World;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP,
};

// 视角默认值
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.5f;
const GLfloat SENSITIVTY = 0.5f;
const GLfloat ZOOM = 65.0f;


class Camera
{
private:
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::mat4 projectionMatrix;
	// 欧拉角
	GLfloat Yaw;
	GLfloat Pitch;
	// 视角移动
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;
public:
	// 位置向量
	glm::vec3 Position;

	bool flying;	
	bool showCurser;	

	// 基于向量的构造函数
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
		this->firstMouse = true;
		this->showCurser = false;
		this->flying = false;
	}

	// 基于标量的构造函数
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// 设定投影矩阵
	void setProjectionMatrix(glm::mat4 projectionMatrix)
	{
		this->projectionMatrix = projectionMatrix;
	}

	// 视图矩阵
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	glm::mat4 getProjectionMatrix()
	{
		return projectionMatrix;
	}

	glm::mat4 getProjViewMatrix()
	{
		return projectionMatrix * GetViewMatrix();
	}

	// 位置移动
	void doMovement(GLboolean Keys[], World* world, GLfloat deltaTime);
	
	// 视野缩放
	void doZoom(GLboolean Keys[], GLfloat deltaTime);

	// 键盘交互
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	// 鼠标交互
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

	// 调用鼠标事件接口
	void camera_mouse_callback(int xpos, int ypos);

	// 获取当前视野比例
	GLfloat getZoom();

	// 实现
	void setLookAt();

private:

	// 鼠标位置判定
	bool firstMouse;
	int lastX;
	int lastY;
	
	// 根据欧拉角计算视角并更新
	void updateCameraVectors();

	// 碰撞检测
	bool doCollisions(World* world);
};

#endif