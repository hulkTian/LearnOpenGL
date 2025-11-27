#version 300 es
precision mediump float;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;//点光源位置

    vec3 ambient;//环境光强度
    vec3 diffuse;//漫反射光强度
    vec3 specular;//镜面光强度

    float constant;//常数衰减因子
    float linear;//线性衰减因子
    float quadratic;//二次衰减因子
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // 计算环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // 计算漫反射
    vec3 norm = normalize(Normal);
    //使用点光源计算光的方向
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // 计算镜面高光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // 计算衰减
    float distance    = length(light.position - FragPos);//片段到光源的距离
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 点光源，最终的片段颜色乘以衰减值
    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);

}