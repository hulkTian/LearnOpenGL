//
// Created by ts on 2024/12/11.
//

#ifndef LEARNOPENGL_PARTICLEGENERATOR_H
#define LEARNOPENGL_PARTICLEGENERATOR_H

#include <GLES3/gl32.h>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"
#include <vector>

// Represents a single particle and its state
// Particle结构体代表一个单独的粒子和它的状态
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time
// ParticleGenerator 作为一个容器来存放需要渲染的大量粒子，这些粒子会重复生成和更新并且会在给定的时间消亡。
class ParticleGenerator {
public:
    // Constructor
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);

    // Update all particles
    // 更新所有粒子
    void Update(GLfloat dt, GameObject &object, GLuint newParticles,
                glm::vec2 offset = glm::vec2(0.0f, 0.0f));

    // Render all particles
    // 渲染所有粒子
    void Draw();

private:
    std::vector<Particle> particles;
    GLuint amount;
    Shader shader;
    Texture2D texture;
    GLuint VAO;

    // Initializes buffer and vertex attributes
    // 初始化缓冲区和顶点属性
    void init();

    // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle
    // is currently inactive
    // 返回当前首个没有被用到的粒子。生命小于等于0.0的粒子或如果当前没有激活的粒子返回0
    GLuint firstUnusedParticle();

    // Respawns particle
    // 重置粒子
    void respawnParticle(Particle &particle, GameObject &object,
                         glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif //LEARNOPENGL_PARTICLEGENERATOR_H
