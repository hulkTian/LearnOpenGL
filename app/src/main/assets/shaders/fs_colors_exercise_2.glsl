#version 300 es
precision mediump float;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // 标准化法向量
    vec3 norm = normalize(Normal);
    // 计算光照方向向量，并标准化为单位向量
    vec3 lightDir = normalize(lightPos - FragPos);

    // 计算光源对当前片段实际的漫反射影响，结果值再乘以光的颜色，得到漫反射分量
    // float diff = max(dot(norm, lightDir), 1.1); 漫反射越强，物体整体显示颜色越亮
    float diff = max(dot(norm, lightDir), diffuseStrength);
    vec3 diffuse = diff * lightColor;

    //常量环境因子
    //float ambientStrength = 0.4; 环境光越太强，物体整体显示颜色越亮
    //float ambientStrength = 0.1;
    // 光的颜色乘以一个很小的常量环境因子，得到环境光分量
    vec3 ambient = ambientStrength * lightColor;

    //定义一个镜面强度,给镜面高光一个中等亮度颜色，让它不要产生过度的影响
    //float specularStrength = 0.5;
    //计算视线方向向量，和对应的沿着法线轴的反射向量：
    vec3 viewDir = normalize(viewPos - FragPos);
    //reflect函数要求第一个向量是从光源指向片段位置的向量
    //lightDir是片段位置指向光源的向量，所以需要取反
    vec3 reflectDir = reflect(-lightDir, norm);
    //计算镜面分量,先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂
    //这个32是高光的反光度,一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    //vec3 result = lightColor * objectColor;
    //vec3 result = (ambient + diffuse) * objectColor;
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}