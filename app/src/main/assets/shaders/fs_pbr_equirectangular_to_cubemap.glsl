#version 300 es
// 设置浮点数精度为中等精度，在精度和性能间做平衡，常用于移动设备等资源受限环境
precision mediump float;

// 定义片段着色器的输出变量，用于传递最终颜色值给渲染管线后续阶段，vec4表示四维向量，包含RGB颜色通道和透明度通道
out vec4 FragColor;
// 从顶点着色器传入的输入变量，代表物体在世界空间中的三维位置坐标
in vec3 WorldPos;

// 定义一个二维纹理采样器统一变量，用于存储等距柱状投影纹理的数据，后续可从中采样颜色
uniform sampler2D equirectangularMap;

// 定义一个常量二维向量，用于后续对纹理坐标进行特定的缩放调整
const vec2 invAtan = vec2(0.1591, 0.3183);

// 函数定义：根据输入的三维向量v计算用于在等距柱状投影纹理上采样的二维纹理坐标（UV坐标）
vec2 SampleSphericalMap(vec3 v)
{
    // 第一步：根据输入向量v的各个分量，使用三角函数计算出一个临时的二维向量uv，作为初始纹理坐标
    // atan(v.z, v.x)计算水平方向角度相关值，asin(v.y)计算垂直方向角度相关值，共同构成基于球坐标系统角度表示的初始纹理坐标
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));

    // 第二步：对计算得到的初始纹理坐标uv进行缩放操作，使其更符合等距柱状投影纹理的采样要求
    // 将uv与预先定义的invAtan向量相乘，在水平和垂直方向上进行特定的缩放调整
    uv *= invAtan;

    // 第三步：将缩放后的纹理坐标uv进行偏移操作，将其范围调整到[0, 1]的标准纹理坐标范围，以便正确在二维纹理上采样
    uv += 0.5;

    // 返回计算并调整好的二维纹理坐标uv
    return uv;
}

void main()
{
    // 将立方体的局部坐标进行插值得到采样方向向量，并对其进行归一化操作，以便在基于角度的纹理坐标计算中得到正确结果
    // 归一化后的向量可更方便地表示方向，然后调用SampleSphericalMap函数计算用于采样等距柱状投影纹理的二维纹理坐标uv
    vec2 uv = SampleSphericalMap(normalize(WorldPos));

    // 根据计算得到的纹理坐标uv，从等距柱状投影纹理equirectangularMap中采样获取相应位置的颜色值，只获取RGB三个通道的值
    vec3 color = texture(equirectangularMap, uv).rgb;

    // 将采样得到的颜色值color包装成一个四维向量，添加透明度为1.0的分量，表示完全不透明
    // 然后将该四维向量赋值给FragColor，以便将最终颜色值传递给渲染管线后续阶段，在屏幕上显示出带有相应纹理外观的物体
    FragColor = vec4(color, 1.0);
}