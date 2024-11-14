//
// Created by ts on 2024/11/12.
//

#ifndef LEARNOPENGL_NORMAL_MAPPING_H
#define LEARNOPENGL_NORMAL_MAPPING_H

#include "GLBaseSample.h"

class normal_mapping : public GLBaseSample {
public:
    normal_mapping() = default;
    virtual ~normal_mapping() = default;

   virtual void Create();
   virtual void Draw();
   virtual void Shutdown();

private:
    GLuint  diffuseMap, normalMap;
    // lighting info
    // -------------
    glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
    GLuint quadVAO,quadVBO;

    void renderQuad();
};


#endif //LEARNOPENGL_NORMAL_MAPPING_H
