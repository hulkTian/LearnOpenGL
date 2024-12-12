//
// Created by ts on 2024/12/10.
//

#ifndef LEARNOPENGL_BALLOBJECT_H
#define LEARNOPENGL_BALLOBJECT_H

#include "GameObject.h"

class BallObject : public GameObject {
public:
    // 球的状态
    GLfloat Radius;
    GLboolean Stuck;
    // 球是否可以粘在挡板上，球是否可以穿过非实心砖块
    GLboolean Sticky, PassThrough;

    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};


#endif //LEARNOPENGL_BALLOBJECT_H
