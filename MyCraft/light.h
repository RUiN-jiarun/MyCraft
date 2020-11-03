#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include "GL/glew.h"
#include<stdio.h>
#include "game.h"

// 光照类
class Light {
protected:
	GLenum mLightIdentifier;
	Light();
public:
	void SetAmbientColor(float r, float g, float b, float a);	// 设置环境光颜色
	void SetDiffuseColor(float r, float g, float b, float a);	// 设置漫反射光颜色（散射）
	void SetSpecularColor(float r, float g, float b, float a);	// 设置镜面反射光颜色（高光）
	void Enable();
};

// 方向光
class DirectionLight :public Light {
public:
	DirectionLight(GLenum light);
	void SetPosition(float x, float y, float z);	// 光源位置
};

// 点光源
class PointLight :public Light {
public:
	PointLight(GLenum light);						
	void SetPosition(float x, float y, float z);	// 光源位置
	void SetConstAttenuation(float v);				// 常数衰减系数（与距离无关）
	void SetLinearAttenuation(float v);				// 线性衰减函数
	void SetQuadricAttenuation(float v);			// 平方衰减函数
};

// 聚光灯
class SpotLight :public PointLight {
public:
	SpotLight(GLenum light);
	void SetDirection(float x, float y, float z);
	void SetExponent(float v);						// 聚光程度
	void SetCutoff(float v);						// 角度
};

#endif