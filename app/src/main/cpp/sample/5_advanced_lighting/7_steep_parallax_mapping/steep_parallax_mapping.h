//
// Created by ts on 2024/11/14.
//

#ifndef LEARNOPENGL_STEEP_PARALLAX_MAPPING_H
#define LEARNOPENGL_STEEP_PARALLAX_MAPPING_H

#include "GLBaseSample.h"

class steep_parallax_mapping : public GLBaseSample {
public:
    steep_parallax_mapping()=default;

    virtual ~steep_parallax_mapping()=default;
    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    GLuint diffuseMap, normalMap, heightMap;
    // lighting info
    // -------------
    glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
    GLuint quadVAO,quadVBO;
    float heightScale = 0.1f;
    void renderQuad();
};
#endif //LEARNOPENGL_STEEP_PARALLAX_MAPPING_H
