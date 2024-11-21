//
// Created by ts on 2024/11/21.
//

#ifndef LEARNOPENGL_LIGHTING_TEXTURED_H
#define LEARNOPENGL_LIGHTING_TEXTURED_H

#include "GLBaseSample.h"

class lighting_textured : public GLBaseSample {

public:

    lighting_textured() = default;

    virtual ~lighting_textured() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint sphereVAO;
    GLuint albedo, normal, metallic, roughness, ao;
    unsigned int indexCount;
    // lights
    // ------
    glm::vec3 lightPositions[4];
    glm::vec3 lightColors[4];
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;
    glm::mat4 projection;

    void renderSphere();
};


#endif //LEARNOPENGL_LIGHTING_TEXTURED_H
