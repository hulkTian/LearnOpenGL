#version 320 es
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fcolor;

void main() {
    fcolor = gs_in[0].color; // gs_in[0] 因为只有一个输入顶点

    gl_Position = gl_in[0].gl_Position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:右下
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:左上
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:右上
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.4, 0.0, 0.0);    // 5:顶部
    fcolor = vec3(1.0, 1.0, 1.0); // 顶部顶点颜色为白色
    EmitVertex();

    EndPrimitive();
}