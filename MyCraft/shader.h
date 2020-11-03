#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// 着色器加载、编译类
class Shader
{
public:
	// 生成ID
	GLuint ID;

	// 激活当前着色器
	Shader& Use();

	// 编译着色器，默认几何着色器为null
	void    Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); 
	
	// 下面为Uniform的Set函数
	void    SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
	void    SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void    SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void    SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);

private:
	// 检查编译错误
	void    checkCompileErrors(GLuint object, std::string type);
};


#endif

