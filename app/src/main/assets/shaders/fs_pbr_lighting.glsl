#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 camPos;

// 反照率
uniform vec3 albedo;
// 金属度
uniform float metallic;
// 粗糙度
uniform float roughness;
// 环境光遮蔽
uniform float ao;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

const float PI = 3.14159265359;

// 菲涅尔方程
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// 法线分布函数
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

// 几何函数
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    //遍历每个光源，计算辐射率总和
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 4; i++)
    {
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);

        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // 大多数绝缘体的F0为0.04
        vec3 F0 = vec3(0.04);
        // 根据初始的F0和表现金属属性的反射率进行线性插值
        F0 = mix(F0, albedo, metallic);
        // 计算DFG
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        //计算BRDF镜面反射部分的比率，分母项中加了一个0.001为了避免出现除零错误
        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular     = nominator / denominator;

        // kS 表示被光能中反射的能量的比例
        vec3 kS = F;
        // 光能中折射的比值 kD
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        // Cook-Torrance BRDF模型，计算出射光线的辐射率
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
    vec3 ambient = vec3(0.03) * albedo * ao;
    // 加一个环境光照项给Lo，然后我们就拥有了片段的最后颜色
    vec3 color   = ambient + Lo;

    // 直到现在，我们假设的所有计算都在线性的颜色空间中进行的，因此我们需要在着色器最后做伽马矫正。
    // Lo作为结果可能会变大得很快(超过1)，但是因为默认的低动态范围（LDR）而取值被截断。所以在伽马矫正之前我们采用色调映射使Lo从LDR的值映射为HDR的值。
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}