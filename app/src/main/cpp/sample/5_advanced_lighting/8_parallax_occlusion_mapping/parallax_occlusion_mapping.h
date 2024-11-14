//
// Created by ts on 2024/11/14.
//

#ifndef LEARNOPENGL_PARALLAX_OCCLUSION_MAPPING_H
#define LEARNOPENGL_PARALLAX_OCCLUSION_MAPPING_H

#include "GLBaseSample.h"

class parallax_occlusion_mapping: public GLBaseSample {
public:
    parallax_occlusion_mapping()=default;

    virtual ~parallax_occlusion_mapping() = default;
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


#endif //LEARNOPENGL_PARALLAX_OCCLUSION_MAPPING_H
