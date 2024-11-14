#version 300 es
precision mediump float;
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale;

// 普通的视差贴图计算位移
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    float height =  texture(depthMap, texCoords).r;
    return texCoords - viewDir.xy * (height * heightScale);
}

// 解决普通视差贴图计算中存在的陡峭的高度导致的不真实的问题
vec2 SteepParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    // 根据观察向量和法向量的夹角给高度分层：当观察向量越偏向平面，需要显示的高度差精度越高，分层就越多
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // 每一层的深度值
    float layerDepth = 1.0 / numLayers;
    // 当前层的深度值
    float currentLayerDepth = 0.0;
    // 计算P向量
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    // 根据P向量和层数计算需要偏移的纹理坐标差
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

    // 遍历每一层深度，直到找到当前层深度大于采样深度的纹理坐标
    while(currentLayerDepth < currentDepthMapValue)
    {
        // 沿着P向量的方向移动当前纹理坐标
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    return currentTexCoords;
}

// 对多次采样的结果进行线性插值计算，解决前面的计算方式出现的纹理断层和锯齿
vec2 ParallaxOcclusionMapping(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    // 根据观察向量和法向量的夹角给高度分层：当观察向量越偏向平面，需要显示的高度差精度越高，分层就越多
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // 每一层的深度值
    float layerDepth = 1.0 / numLayers;
    // 当前层的深度值
    float currentLayerDepth = 0.0;
    // 计算P向量
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    // 根据P向量和层数计算需要偏移的纹理坐标差
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

    // 遍历每一层深度，直到找到当前层深度大于采样深度的纹理坐标
    while(currentLayerDepth < currentDepthMapValue)
    {
        // 沿着P向量的方向移动当前纹理坐标
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = TexCoords;

    //texCoords = ParallaxMapping(TexCoords,  viewDir);
    //texCoords = SteepParallaxMapping(TexCoords,  viewDir);
    texCoords = ParallaxOcclusionMapping(TexCoords,  viewDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    discard;

    // obtain normal from normal map
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    // get diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}