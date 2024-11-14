#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
// 从技术上讲，顶点着色器中无需副切线。
// 三个TBN向量都是相互垂直的可以用T和N向量的叉乘计算出： vec3 B = cross(T, N);
// layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    /**
    * 计算TBN矩阵：
    * 1.首先计算模型矩阵的逆转置矩阵，并存储在normalMatrix中，这个矩阵通常用于将法线从模型空间转换到世界空间。
    * 2.通过将输入的切线向量aTangent与normalMatrix相乘，然后进行归一化，得到切线向量T在世界空间中的表示。
    * 3.类似地，将输入的法线向量aNormal与normalMatrix相乘并归一化，得到法线向量N在世界空间中的表示。
    * 4.对切线向量T进行调整，使其与法线向量N垂直。首先计算T和N的点积，然后将其乘以N，从T中减去这个结果并再次归一化。
    * 5.通过计算法线向量N和调整后的切线向量T的叉积，得到副切线向量（也称为副法线向量或双切线向量）B。
    */
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);//单位向量，切线向量T
    vec3 N = normalize(normalMatrix * aNormal);//单位向量，法向量N
    /**
    * 模型中为了节省顶点常常有很多共享的顶点，这可能会导致TBN矩阵不能相互垂直，需要进行重新正交化。
    * 使用格拉姆-施密特正交化重新计算正交的TBN矩阵：
    * 1.dot(T, N) = |T| * |N| * cosθ = cosθ 即T和N夹角的余玄
    * 2.dot(T, N) * N = cosθ * N 即T在N向量上的投影向量
    * 3.T - cosθ * N 减去了T在N方向的分量，留下T垂直N的分量
    * 4.normalize(T - dot(T, N) * N) 归一化，计算出垂直N向量的单位向量T
    */
    T = normalize(T - dot(T, N) * N);
    // 叉乘N和T，计算垂直与T和N的副切线向量B，也是一个单位向量
    vec3 B = cross(N, T);

    //通过TBN矩阵的逆矩阵将顶点位置、摄像机位置、光源位置都置换到切线空间，并传递给片元着色器
    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}