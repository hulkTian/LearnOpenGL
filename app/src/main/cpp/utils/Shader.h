//
// Created by ts on 2024/12/6.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include <string>
#include <GLES3/gl32.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// General purpose shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility
// functions for easy management.
// 通用的着色器程序对象。从文件编译，生成编译/链接时错误信息并实现一些实用方法便于管理。
class Shader
{
public:
    // State
    // 着色器程序对象句柄
    GLuint ID;
    // Constructor
    // 构造函数
    Shader() { }
    // Sets the current shader as active
    // 激活当前着色器程序
    Shader  &Use();
    // Compiles the shader from given source code
    // 从着色器源代码编译着色器程序
    // Note: geometry source code is optional
    // 注意：几何着色器源码是可选的
    void    Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);
    // Utility functions
    // 实用方法
    void    SetFloat    (const GLchar *name, GLfloat value, GLboolean useShader = false);
    void    SetInteger  (const GLchar *name, GLint value, GLboolean useShader = false);
    void    SetVector2f (const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void    SetVector2f (const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
    void    SetVector3f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void    SetVector3f (const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
    void    SetVector4f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void    SetVector4f (const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
    void    SetMatrix4  (const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
    // Checks if compilation or linking failed and if so, print the error logs
    // 检查编译或链接是否失败，如果失败，则打印错误日志
    void checkCompileErrors(GLuint object, std::string type);
};


#endif //LEARNOPENGL_SHADER_H
