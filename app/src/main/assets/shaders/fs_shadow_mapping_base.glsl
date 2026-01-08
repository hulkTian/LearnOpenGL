#version 300 es
precision mediump float;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;// 光空间的片段位置

uniform sampler2D diffuseTexture;
uniform samplerCube depthCubeMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int type;

/**
* 判断片段是否在阴影中：
* 1. 光空间片段位置转换为裁切空间的标准化设备坐标，返回了片段在光空间的-1到1的范围；
* 2. NDC坐标变换为0到1的范围，方便从深度贴图中去采样；
* 3. 使用projCoords坐标对深度贴图采样，得到光的位置视野下最近的深度；
* 4. 获取当前片段的Z轴坐标作为当前片段深度；
* 5. 通过深度比较，判断当前片段是否在阴影中；
*/
float ShadowCalculation(vec4 fragPosLightSpace) {
    // 执行透视除法，把光空间的片段坐标转换到裁切空间的标准化设备坐标，范围是[-1, 1]
    // 当使用正交投影矩阵时，w分量恒为1.0，所以这一步没有任何影响，但是当使用透视投影矩阵时，这一步是必须的，所以保留这一行。
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 为了方便对深度贴图进行采样，把光空间的NDC坐标转换到[0, 1]范围
    projCoords = projCoords * 0.5 + 0.5;
    // todo [过采样]优化方案第二步：超出光源视锥体空间的区域，Z坐标都会大于1.0，这些区域可以直接显示正常片段纹理
    if (projCoords.z > 1.0) {
        return 0.0;
    }
    // 获取光空间的最近深度值
    float closestDepth = texture(depthCubeMap, projCoords.xy).r;
    // 获取当前片段的深度值
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    if (type == 0) {
        // todo 这种比较会出现阴影失真，原因是光源太远，且阴影贴图分辨率有限，多个片段可能从相同深度值中采样
        float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
        return shadow;
    } else if (type == 1) {
        // todo 解决阴影失真方案一：深度偏移
        float bias = 0.005;
        float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
        return shadow;
    } else if (type == 2) {
        // 偏移量依赖光源与表面的夹角，如果倾角变大，偏移量也要变大，以避免阴影失真
        // todo 解决阴影失真方案二：使用点乘动态偏移
        vec3 lightDir = normalize(lightPos - FragPos);
        float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);
        float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
        return shadow;
    } else if (type == 3) {
        // todo 解决阴影悬浮：使用点乘动态偏移同时开启正面剔除
        // 效果不大，通过调整bias 也能达到类似效果
        vec3 lightDir = normalize(lightPos - FragPos);
        float bias = max(0.01 * (1.0 - dot(Normal, lightDir)), 0.005);
        float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
        return shadow;
    } else if (type == 4) {
        vec3 lightDir = normalize(lightPos - FragPos);
        float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);
        // todo 解决阴影失真方案三：PCF，百分比多重采样
        float shadow = 0.0;
        // textureSize 函数返回纹理在0级mipmap下的宽度和高度
        ivec2 textureSizeResult = textureSize(shadowMap, 0);
        // 取倒数得到单个纹理的像素大小，可以理解为纹理坐标系下的偏移量
        vec2 texelSize = 1.0 / vec2(textureSizeResult);
        // 在片段周边采样九次深度值
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                // vec2(x, y) * texelSize 一个纹理的纹理坐标大小
                // projCoords.xy + vec2(x, y) * texelSize 把projCoords.xy位移到指定的采样位置
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }
        shadow /= 9.0;
        return shadow;
    }
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
    // 2) 不在阴影中时，显示第2点的颜色就行(因为光散射的缘故，阴影很少全黑，所以没有让环境分量乘以阴影分量)
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    FragColor = vec4(lighting, 1.0);
}