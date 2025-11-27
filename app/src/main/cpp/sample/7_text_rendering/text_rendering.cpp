//
// Created by ts on 2024/12/4.
//

/**
 *
 * 使用FreeType库渲染文本：Free Type所做的事就是加载TureType字体并为每一个字形生成位图以及计算几个度量值（Metric）。
 * 我们可以提取出它生成的位图作为字形的纹理，并使用这些度量值定位字符的字形。
 *
 * 编译和加载FreeType库：
 * 1. 使用python脚本构建交叉编译环境
 * 2. 编写编译动态库的脚本，执行后生成动态库
 * 3. 把对应平台的动态库放到对应的平台lib目录
 * 4. 再makefile 文件中定义动态库应用路径和名称 libfreetype
 * 5. 把动态库的名字加到链接函数中
 *
 * 使用FreeType库加载字体并渲染：
 * 1. 初始化FreeType 库，等到FT_Library对象
 * 2. 加载字体库，获取FT_Face对象
 * 3. 使用FT_Face对象，定义字体大小
 * 4. 加载字符的字形，获取字形度量值和位图，将位图绑定到纹理（本例只加载了ASCII的前128个字符）
 * 5. 使用四边形渲染字形的纹理
 *
 * 使用FreeType从字体加载的字形位图大小是不同的，所以需要一些度量值来定义字形位图：
 * 1. width        位图宽度（像素）                                         face->glyph->bitmap.width
 * 2. height       位图高度（像素）                                         face->glyph->bitmap.rows
 * 3. bearingX     水平距离，位图左边相对于原点的水平位置（像素）                 face->glyph->bitmap_left
 * 4. bearingY     垂直距离，位图顶边相对于基准线的垂直位置（像素）               face->glyph->bitmap_top
 * 5. advance      水平预留值，原点到下一个字形原点的水平距离（单位：1/64像素）     face->glyph->advance.x
 */

#include "text_rendering.h"

REGISTER_SAMPLE(SAMPLE_TYPE_TEXT_RENDERING, text_rendering)

void text_rendering::Create()
{
    // 重要：需要开启混合，否则最后会显示纯色的四边形
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_ProgramObj = GLUtils::createProgram("shaders/vs_text_rendering.glsl", "shaders/fs_text_rendering.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    // 初始化FreeType库
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        LOGE("ERROR: FREETYPE: Cloud not init FreeType Library")
    }

    // 字体加载到FreeType定义的面（Face），字体面包含了字体中所有的字形
    // 使用FT_New_Face函数时，需要传递字体的存放路径，所以字体要放在SD卡目录才能直接通过路径进行访问
    FT_Face face;
    if (FT_New_Face(ft, "/data/data/com.example.learnopengl/fonts/HarmonyOS_Sans_SC_Medium.ttf", 0, &face))
    {
        LOGE("ERROR: FREETYPE: Failed to load font")
    }

    // 这个函数用于设置字体面的宽高，将宽设置为0表示要从字体面的高度中动态计算出字形的宽度
    FT_Set_Pixel_Sizes(face, 0, 48);

    text1 = L"渲染 This is sample text";
    loadText(face, text1);
    text2 = L"使徒来袭 \nmashiro-真白-ましろ \n Ciallo～(∠・ω< )⌒★\n 天动万象 I will have order";
    loadText(face, text2);

    // 加载完需要的字形后需要释放FreeTpe
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // VBO 是 NULL，不同字符的度量值不同，对应的顶点坐标不同，所以先申请一个sizeof(GLfloat) * 6 * 4的缓冲区
    // GL_DYNAMIC_DRAW 定义缓冲区类型，因为需要频繁更新VBO的数据来渲染每个字符
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    // 每个顶点属性包含两个顶点坐标和两个纹理坐标，构成要给vec4，这样就可以用一个顶点属性传递所有需要的顶点数据
    // 顶点坐标只有X和Y轴，本例只是做2维的字体渲染
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    // 恢复默认绑定，解绑VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 为着色器设置一个正射投影矩阵，因为文本渲染不需要透视
    glUseProgram(m_ProgramObj);
    glm::mat4 projection = glm::ortho(0.0f, SCR_WIDTH, 0.0f, SCR_HEIGHT);
    setMat4(m_ProgramObj, "projection", projection);
}

void text_rendering::Draw()
{
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderText(text1, 25.0f, 25.0f, 4.0f, glm::vec3(0.5, 0.8f, 0.2f));
    RenderText(text2, 1000.0f, 1000.0f, 1.1f, glm::vec3(0.3, 0.7f, 0.9f));
}

void text_rendering::loadText(const FT_Face face, const std::wstring text)
{
    // 字符只需要一个字节就能存放，而OpenGL的纹理缓冲空间一般是4个字节对齐的。所以这里将对齐方式改为一个字节，OpenGL读取数据时以一个字节作为读取单元
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int len_w = text.length();
    for (int i = 0; i < len_w; i++)
    {
        wchar_t  c = text[i];
        LOGV("loadText: %c", c)
        // 加载字符对应的字形
        // FT_LOAD_RENDER是加载标记之一，作用是告诉FreeType去创建一个8位的灰度位图，我们可以通过face->glyph->bitmap来访问这个位图
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            LOGD("ERROR::FREETYTPE: Failed to load Glyph")
            continue;
        }
        // 生成纹理，将位图数据存放到纹理的缓冲区
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,                    // 纹理数据格式对应字形位图大小
                face->glyph->bitmap.width, // 位图宽度作为纹理的宽度
                face->glyph->bitmap.rows,  // 位图高度作为纹理的高度
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer // 读取位图缓冲区数据（通常是4字节对齐的），保存到纹理缓冲区
        );
        // 设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 储存字符供之后使用
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        // 字符的度量值使用映射表存储，key就是字符的值，方便后续根据字符索引
        Characters.insert(std::pair<wchar_t, Character>(c, character));
    }
}

/**
 * 渲染一行文本
 * @param text    需要渲染的文本
 * @param startX       文本的X轴坐标
 * @param startY       文本的Y轴坐标
 * @param scale   文本的缩放比例
 * @param color   文本颜色
 */
void
text_rendering::RenderText(std::wstring text, GLfloat startX, GLfloat startY, GLfloat scale, glm::vec3 color)
{
    glUseProgram(m_ProgramObj);
    setVec3(m_ProgramObj, "textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // 遍历字符串，渲染每个字符
    std::wstring::const_iterator c;
    GLfloat y = startY, x = startX;
    for (c = text.begin(); c != text.end(); c++)
    {
        if (*c == '\n')
        {
            y -= 48 * scale;
            x = startX;
        }
        else
        {

            // 从映射表获取字符度量值
            Character ch = Characters[*c];

            // X轴偏移值 = 字符X轴坐标 + 字符左边与基线的距离 * 缩放值
            GLfloat xpos = x + ch.Bearing.x * scale;
            // Y轴偏移值 = 字符Y轴坐标 - 左边距 * 缩放值
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat h = ch.Size.y * scale;
            GLfloat w = ch.Size.x * scale;
            // 构建顶点数据
            GLfloat vertices [6][4] = {
                    {xpos, ypos + h, 0.0, 0.0},
                    {xpos, ypos, 0.0, 1.0},
                    {xpos + w, ypos, 1.0, 1.0},

                    {xpos, ypos + h, 0.0, 0.0},
                    {xpos + w, ypos, 1.0, 1.0},
                    {xpos + w, ypos + h, 1.0, 0.0},
            };
            // 在四边形上绘制字形纹理
            glBindTexture(GL_TEXTURE_2D, ch.TexturedID);
            // 更新VBO内存的内容
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // 绘制四边形，相当于绘制一个字符
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // 绘制完一个纹理后，其实坐标需要根据当前字符的advance进行偏移，由于所有字符都在一行，所以只需要改变X轴即可
            // 单位：1/64像素，所以需要转换为像素再进行缩放
            x += (ch.Advance >> 6) * scale;
        }
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void text_rendering::Shutdown() {
    GLBaseSample::Shutdown();
}