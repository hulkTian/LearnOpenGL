#version 300 es
precision mediump float;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_height1;
    float shininess;
};
uniform Material material;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform samplerCube skybox;

// 生成定向光
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    // 合成反射贴图效果
    vec3 skyReflection = texture(skybox, reflectDir).rgb;
    vec3 reflection = vec3(texture(material.texture_height1, TexCoords)) * skyReflection;
    return (ambient + diffuse + specular + reflection);
}

void main()
{
    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - Position);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
}