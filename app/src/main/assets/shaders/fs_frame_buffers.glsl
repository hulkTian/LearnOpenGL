#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void normal() {
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}

// 相反效果
void inversion() {
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}

// 灰度效果
void grayscale() {
    FragColor = texture(screenTexture, TexCoords);
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    // 使用加权的通道，获取物理上更精确的效果
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
}

const float offset = 1.0 / 300.0;
// 核效果
void kernel() {
    vec2 offsets[9] = vec2[](
    vec2(-offset, offset), // 左上
    vec2(0.0f, offset), // 正上
    vec2(offset, offset), // 右上
    vec2(-offset, 0.0f), // 左
    vec2(0.0f, 0.0f), // 中
    vec2(offset, 0.0f), // 右
    vec2(-offset, -offset), // 左下
    vec2(0.0f, -offset), // 正下
    vec2(offset, -offset)// 右下
    );

    // 锐化核数组，数组元素的和通常是1
    /*float kernel[9] = float[](
    -1.0, -1.0, -1.0,
    -1.0, 9.0, -1.0,
    -1.0, -1.0, -1.0
    );*/

    // 模糊核数组
    float kernel[9] = float[](
    1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
    2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
    1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
    );

    // 边缘检测(Edge-detection)核数组
    /**float kernel[9] = float[](
    1.0, 1.0, 1.0,
    1.0, -8.0, 1.0,
    1.0, 1.0, 1.0
    );*/

    // 浮雕内核
    /**float kernel[9] = float[](
    2.0, -1.0, 0.0,
    -1.0, 1.0, 1.0,
    0.0, 1.0, 2.0
    );*/

    vec3 sampleTex[9];
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        // 获取当前像素和周围像素的纹理颜色
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        // 这个核取了8个周围像素值，将它们乘以-1，而把当前的像素乘以9，实现锐化效果
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}

void main()
{
    //normal();
    kernel();
    //grayscale();
}