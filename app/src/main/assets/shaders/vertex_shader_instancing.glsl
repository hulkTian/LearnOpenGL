#version 300 es
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

uniform vec2 offsets[100];

void main()
{
    fColor = aColor;
    // 从右上到左下逐渐缩小四边形：
    vec2 pos = aPos * (float(gl_InstanceID) / 100.0);
    gl_Position = vec4(pos + aOffset, 0.0, 1.0);
}
