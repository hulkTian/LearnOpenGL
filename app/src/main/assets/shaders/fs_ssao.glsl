#version 300 es
precision mediump float;

out float FragColor;

in vec2 TexCoords;

// gBuffer 保存的顶点位置向量
uniform sampler2D gPosition;
// gBuffer 保存的法线向量
uniform sampler2D gNormal;
// 切线空间的绕 Z 轴随机旋转的向量
uniform sampler2D texNoise;
// 切线空间中随机64个采样点，非线性分布，离核心越近采样点越多
uniform vec3 samples[64];

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;//控制样本数
float radius = 0.5;// 控制采样半径
float bias = 0.025;// 控制深度阈值，确认有效样本

// 基于屏幕分辨率对噪声纹理进行平铺
const vec2 noiseScale = vec2(2560.0/4.0, 1680.0/4.0);
// 投影矩阵
uniform mat4 projection;

void main()
{
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    // 先对平铺到屏幕上的噪声纹理进行采样，获得垂直于Z轴的随机向量
    // 在对向量进行归一化，使向量范围映射的到[-1,1]
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // randomVec本来是在切线空间垂直于Z轴的，现在作为切线来进一步计算垂直于法线的切线
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    // 计算垂直于切线和法线的副切线
    vec3 bitangent = cross(normal, tangent);
    // 构建TBN矩阵
    mat3 TBN = mat3(tangent, bitangent, normal);
    // 迭代样本核并计算遮蔽因子
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // 获取样本位置
        vec3 samplePos = TBN * samples[i]; // 切线->观察空间
        // 计算观察空间中片段对应的采样位置：先对样本位置进行缩放，保证在有效半径采样；再将片段移动到需要采样的位置
        samplePos = fragPos + samplePos * radius;

        // 将片段位置移动到裁剪空间，用于采样深度
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // 观察->裁剪空间
        offset.xyz /= offset.w; // 透视划分
        offset.xyz = offset.xyz * 0.5 + 0.5; // 变换到0.0 - 1.0的值域

        // 使用屏幕空间中的采样坐标对片段进行采样，获取深度值
        float sampleDepth = texture(gPosition, offset.xy).z;

        // 如果简单地将超出有效半径的样本完全舍弃，可能会导致在有效半径边界处出现明显的视觉断层。
        // 通过使用smoothstep函数返回一个 0 - 1 之间的插值，可以让超出有效半径的样本以一种逐渐衰减的方式影响最终结果。
        // 当fragPos.z - sampleDepth 刚刚超过radius时，返回一个接近 1 的值，表示样本仍然有较大的影响；
        // 随着fragPos.z - sampleDepth与radius的差值越来越大，返回值会逐渐减小到 0，使得样本的影响平滑地过渡到零，从而避免了突然的视觉变化。
        // 当x >= max时，函数返回1.0
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        // 比较采样的深度和当前片段深度加上一个控制阈值，判断深度是否有效，如果有效平滑的缩放，最后累加遮蔽因子
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    // 将遮蔽贡献根据核心的大小标准化，并输出结果。
    // 1.0减去了遮蔽因子，以便直接使用遮蔽因子去缩放环境光照分量。
    // 当遮蔽因子接近1.0时，说明遮蔽程度高，occlusion 就接近0.0，环境光就会变暗;
    // 当遮蔽因子接近0.0时，说明遮蔽程度低，occlusion 就接近1.0，环境光就会变亮;
    occlusion = 1.0 - (occlusion / float(kernelSize));

    FragColor = occlusion;
}