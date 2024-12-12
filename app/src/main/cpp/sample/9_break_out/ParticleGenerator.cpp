//
// Created by ts on 2024/12/11.
//

#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
        : shader(shader), texture(texture), amount(amount) {
    this->init();
}

/**
 * 更新所有粒子
 * @param dt 每帧时间，更新频率
 * @param object
 * @param newParticles 生成新粒子的数量
 * @param offset 粒子的
 */
void
ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset) {
    // 生成新粒子
    for (GLuint i = 0; i < newParticles; ++i) {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
    }
    // 更新所有粒子
    for (GLuint i = 0; i < this->amount; ++i) {
        // 从粒子数组中按照索引获取粒子对象的引用
        Particle &p = this->particles[i];
        p.Life -= dt;
        // 只更新存活的粒子
        if (p.Life > 0.0f) {
            // 更新粒子的位置，位置= 初始位置 - 速度 * 每帧时间
            p.Position -= p.Velocity * dt;
            // 更新粒子的透明度，随着时间透明度线性变化到0
            p.Color.a -= dt * 2.5;
        }
    }
}

void ParticleGenerator::Draw() {
    // 配置混合方法，使用加性混合来赋予它 “辉光” 效果。
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    for (Particle particle: this->particles) {
        if (particle.Life > 0.0f) {
            this->shader.SetVector2f("offset", particle.Position);
            this->shader.SetVector4f("color", particle.Color);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // 恢复默认混合模式
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
    GLuint VBO;
    GLfloat particle_quad[] = {
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // 填充网格缓冲区
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // 设置网格顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
    glBindVertexArray(0);

    // 创建 amount 个默认的初始粒子
    for (GLuint i = 0; i < this->amount; ++i) {
        this->particles.push_back(Particle());
    }
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle() {
    // 首先从最后一个被用到的粒子开始查找，如果满足条件会快速找到一个没有用到的粒子
    for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // 如果上一步没有找到，说明数组中最后一个粒子就是最后用到的粒子，就从数组第一个元素开始查找
    for (GLuint i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // 如果上面两步都没有找到，说明所有粒子都被使用了，这是重置第一个粒子。
    lastUsedParticle = 0;
    return 0;
}

/**
 * 重置粒子
 * @param particle 需要重置的粒子
 * @param object 需要粒子效果的游戏物体
 * @param offset 粒子需要偏移的位置
 */
void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset) {
    // 随机给一个在 -5.0f 到 4.9f 之间的位置，增加粒子初始位置的随机性
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    // 随机给一个大于0.5的颜色值
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    // 基于游戏里的物体，给粒子设置一个位置
    particle.Position = object.Position + random + offset;
    // 粒子的颜色
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    // 粒子的生命重置为1.0f
    particle.Life = 1.0f;
    // 基于游戏里的物体，给粒子设置一个速度
    particle.Velocity = object.Velocity * 0.1f;
}
