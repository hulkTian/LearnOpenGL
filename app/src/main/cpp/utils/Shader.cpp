//
// Created by ts on 2024/12/6.
//

#include "Shader.h"
#include "LogUtils.h"

#include <iostream>

Shader &Shader::Use() {
    glUseProgram(this->ID);
    return *this;
}

void Shader::Compile(const GLchar *vertexSource, const GLchar *fragmentSource,
                     const GLchar *geometrySource) {
    GLuint sVertex, sFragment, gShader;
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // If geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // Shader Program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(this->ID, gShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

void Shader::SetFloat(const GLchar *name, GLfloat value, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w,
                         GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader) {
    if (useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, std::string type) {
    GLint success;
    if (type != "PROGRAM") {
        // 获取编译状态
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint infoLen = 0;
            //获取错误信息的长度
            glGetShaderiv(object, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                // 分配一块合适长度的内存用于存放错误信息
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                // 获取错误信息内容，写入infoLog
                glGetShaderInfoLog(object, infoLen, nullptr, infoLog);
                LOGE("ERROR::SHADER: Compile-time error: Type:%s\n%s", type.c_str(), infoLog)
                free(infoLog);
            }
        }
    } else {
        // 获取链接状态
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            GLint infoLen = 0;
            glGetProgramiv(object, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(object, infoLen, nullptr, infoLog);
                LOGE("ERROR::Shader: Link-time error:\n%s", infoLog)
                free(infoLog);
            }
        }
    }
}