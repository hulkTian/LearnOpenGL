#version 300 es
precision mediump float;

out vec4 FragColor;

struct Light {
    vec3 position;//光源位置

    vec3 ambient;//环境光的强度
    vec3 diffuse;//漫反射光强度
    vec3 specular;//镜面反射光强度
};

uniform Light light;

void main()
{
    vec3 result = light.ambient + light.diffuse + light.specular;
    FragColor = vec4(result, 1.0);
}