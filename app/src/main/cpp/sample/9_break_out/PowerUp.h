//
// Created by ts on 2024/12/11.
//

#ifndef LEARNOPENGL_POWERUP_H
#define LEARNOPENGL_POWERUP_H

#include "GameObject.h"

const glm::vec2 SIZE(160, 40);
const glm::vec2 VELOCITY(0.0f, 150.0f);

// PowerUp inherits its state and rendering functions from GameObject but also holds extra
// information to state its active duration and whether it is activated or not.
// The type of PowerUp is stored as a string.
// PowerUp类继承了GameObject类的状态和渲染方法，但是也扩展了激活状态时间信息和是否激活的状态。
// 道具类型被作为字符串保存。
class PowerUp : public GameObject {
public:
    // 道具类型
    std::string Type;
    // 道具有效时间
    GLfloat Duration;
    // 道具是否被激活
    GLboolean Activated;

    // 构造函数
    PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
            : GameObject(position, SIZE, texture, color, VELOCITY),Type(type), Duration(duration), Activated() {}
};

#endif //LEARNOPENGL_POWERUP_H
