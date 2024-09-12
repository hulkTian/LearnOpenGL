#version 300 es
precision mediump float;

out vec4 FragColor;

//使用纹理采样漫反射颜色来替代材质中的单色漫反射光分量
//另外，环境光颜色在几乎所有情况下都等于漫反射颜色，所以也移除了环境光分量
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    //练习题：尝试在片段着色器中反转镜面光贴图的颜色值，让木头显示镜面高光而钢制边缘不反光（由于钢制边缘中有一些裂缝，边缘仍会显示一些镜面高光，虽然强度会小很多）：
    //vec3 specular = light.specular * spec * (vec3(1.0) - vec3(texture(material.specular, TexCoords)));

    //vec3 result = ambient + diffuse + specular;
    vec3 result = ambient + diffuse + specular + vec3(texture(material.emission, TexCoords));
    FragColor = vec4(result, 1.0);
}