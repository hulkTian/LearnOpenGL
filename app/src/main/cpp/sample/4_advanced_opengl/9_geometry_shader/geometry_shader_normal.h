//
// Created by tzh on 2025/12/11.
//

#ifndef LEARNOPENGL_GEOMETRY_SHADER_NORMAL_H
#define LEARNOPENGL_GEOMETRY_SHADER_NORMAL_H

#include "GLBaseSample.h"
#include "3_mesh/Model.h"
#include "stb_image.h"

class geometry_shader_normal : public GLBaseSample {
public:
    geometry_shader_normal() = default;

    virtual ~geometry_shader_normal() = default;

    virtual void Create() override;

    virtual void Draw() override;

    virtual void Shutdown() override;

private:
    Model ourModel;
    GLuint m_ProgramObj_Normal;
};


#endif //LEARNOPENGL_GEOMETRY_SHADER_NORMAL_H
