//
// Created by tzh on 2025/12/10.
//

#ifndef LEARNOPENGL_GEOMETRY_SHADER_EXPLODE_H
#define LEARNOPENGL_GEOMETRY_SHADER_EXPLODE_H

#include "GLBaseSample.h"
#include "3_mesh/Model.h"
#include "stb_image.h"

class geometry_shader_explode : public GLBaseSample {
public:
    geometry_shader_explode() = default;

    virtual ~geometry_shader_explode()  = default;

    virtual void Create() override;

    virtual void Draw() override;

    virtual void Shutdown() override;

private:
    Model ourModel;
};


#endif //LEARNOPENGL_GEOMETRY_SHADER_EXPLODE_H
