//
// Created by ts on 2024/12/9.
//

#ifndef LEARNOPENGL_GAMEOBJECT_H
#define LEARNOPENGL_GAMEOBJECT_H

#include <GLES3/gl32.h>
#include "glm/vec2.hpp"
#include "Texture2D.h"
#include "SpriteRenderer.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
// 容器对象用于保存与单个游戏对象实体相关的所有状态。游戏中的每个对象都可能需要GameObject中描述的最小状态。
class GameObject {
public:
// Object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid;
    GLboolean   Destroyed;
    // Render state
    Texture2D   Sprite;
    // Constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // Draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};


#endif //LEARNOPENGL_GAMEOBJECT_H
