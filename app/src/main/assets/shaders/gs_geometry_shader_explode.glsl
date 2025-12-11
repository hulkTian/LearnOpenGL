#version 320 es
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

// 传入时间的统一变量
uniform float time;

out vec2 TexCoords;

// 计算法线的函数
vec3 GetNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

// 计算位移向量的函数
vec4 explode(vec4 position, vec3 normal) {
    float magnitude = 2.0; // 位移的幅度
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; // 根据时间变化的方向
    return position + vec4(direction, 0.0);
}

void main() {
    vec3 normal = GetNormal();

    for (int i = 0; i < 3; ++i) {
        vec4 explodedPosition = explode(gl_in[i].gl_Position, normal);
        gl_Position = explodedPosition;
        TexCoords = gs_in[i].texCoords;
        EmitVertex();
    }
    EndPrimitive();
}