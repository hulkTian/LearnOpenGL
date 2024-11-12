#version 300 es
precision mediump float;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

/**
* 判断片段是否在阴影中：
* 1. 光空间片段位置转换为裁切空间的标准化设备坐标，返回了片段在光空间的-1到1的范围；
* 2. NDC坐标变换为0到1的范围，方便从深度贴图中去采样；
* 3. 使用projCoords坐标对深度贴图采样，得到光的位置视野下最近的深度；
* 4. 获取当前片段的Z轴坐标作为当前片段深度；
* 5. 通过深度比较，判断当前片段是否在阴影中；
*/
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    // todo 这种比较会出现阴影失真，原因是光源太远，且阴影贴图分辨率有限，多个片段可能从相同深度值中采样
    // float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    // 解决方案一：深度偏移，但有些表面坡度大，深度值差异也大，所以还会失真
    // float bias = 0.005;
    // 解决方案二：在深度偏移基础上使用点乘，根据表面朝向光线的角度更改偏移量
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.04 * (1.0 - dot(Normal, lightDir)), 0.005);
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // todo 锯齿优化方案：PCF，百分比多重采样
    float shadow = 0.0;
    // 获取阴影贴图在第0层级的尺寸，返回值是ivec2类型
    ivec2 textureSizeResult = textureSize(shadowMap, 0);
    // 计算一个单独纹理像素的大小
    vec2 texelSize = 1.0 / vec2(textureSizeResult);
    // 在片段周边采样九次深度值
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            // vec2(x, y) * texelSize 一个纹理的纹理坐标大小
            // projCoords.xy + vec2(x, y) * texelSize 把projCoords.xy位移到指定的采样位置
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // todo [采样过多]优化方案第二步：超出光源视锥体空间的区域，Z坐标都会大于1.0，这些区域可以直接显示正常片段纹理
    if (projCoords.z > 1.0) {
        shadow = 0.0;
    }
    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(0.5);
    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // calculate shadow
    float shadow = ShadowCalculation(FragPosLightSpace);
    // 计算片段最后输出的颜色：
    // 1.在没有光源的时候，直接输出 采样的color 就是当前片段的颜色；
    // FragColor = vec4(color, 1.0);
    // 2.有光源但不考虑阴影时，直接使用Blinn-Phong光照模型计算片段在光照影响下的颜色
    //FragColor = vec4((ambient + diffuse + specular) * color , 1.0);
    // 3.有光源且考虑阴影时，需要区分片段是否在阴影中：
    // 1) 在阴影中时，直接降低片段原有颜色的亮度就行
    // 0.3 * lightColor * color
    // 2) 不在阴影中时，显示第2点的颜色就行
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    FragColor = vec4(lighting, 1.0);
}