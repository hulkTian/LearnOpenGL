#version 300 es
precision mediump float;

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    // 计算视线方向向量
    vec3 I = normalize(Position - cameraPos);
    // 计算反射向量
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}