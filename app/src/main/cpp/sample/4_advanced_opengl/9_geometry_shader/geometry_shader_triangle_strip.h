//
// Created by tzh on 2025/12/10.
//

#ifndef LEARNOPENGL_GEOMETRY_SHADER_TRIANGLE_STRIP_H
#define LEARNOPENGL_GEOMETRY_SHADER_TRIANGLE_STRIP_H

#include "GLBaseSample.h"

class geometry_shader_triangle_strip : public GLBaseSample {
public:
    geometry_shader_triangle_strip() = default;

    virtual ~geometry_shader_triangle_strip() = default;

    void Create() override;

    void Draw() override;

    void Shutdown() override;

private:
    unsigned int VAO, VBO;
    unsigned int m_ProgramObj = 0;
};


#endif //LEARNOPENGL_GEOMETRY_SHADER_TRIANGLE_STRIP_H
