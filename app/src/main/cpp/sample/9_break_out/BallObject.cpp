//
// Created by ts on 2024/12/10.
//

#include "BallObject.h"

BallObject::BallObject() : GameObject(), Radius(12.5f), Stuck(true), Sticky(GL_FALSE), PassThrough(GL_FALSE) {

}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
        : GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity),
          Radius(radius), Stuck(true), Sticky(GL_FALSE), PassThrough(GL_FALSE) {

}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width) {
    // 如果没有被固定在挡板上
    if (!this->Stuck) {
        // 移动球
        this->Position += this->Velocity * dt;
        // 检查是否在窗口边界以外，如果是的话反转速度并恢复到正确位置
        if (this->Position.x <= 0.0f) {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        } else if (this->Position.x + this->Size.x >= window_width) {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        // 因为如果球碰触到底部边界时玩家会结束游戏（或失去一条命），所以在底部边界没有代码来控制球反弹。
        if (this->Position.y <= 0.0f) {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    return this->Position;
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = GL_TRUE;
    this->Sticky = GL_FALSE;
    this->PassThrough = GL_FALSE;
}
