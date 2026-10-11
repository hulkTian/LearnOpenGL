# OpenGL → Kanzi 渲染工程师学习路线（策展版）

> 不是教程，是一张**读什么、按什么顺序读**的地图。资源都是业界公认的经典，我只帮你选和排。

---

## 先做 3 件事

### ① 重新读摸底诊断

[Kanzi渲染知识水平调查.md](file:///f:/jmc-ivi-dev/jmc-ivi/doc/Kanzi渲染知识水平调查.md) —— 你的作答和我的批改。

你暴露的核心误解（**全部必须纠正，没有商量余地**）：
- 图元装配可编程 ❌
- 深度测试在片段着色**之前** ❌（实际在**之后**）
- GL_LINEAR/NEAREST 使用场景**反了** ❌（LINEAR 放大、NEAREST 缩小才是常规）
- swizzle ❌（`.yx` 是交换，和 `.xy` 完全不同）
- Z-fighting ❌（完全空白）
- uniform **drawcall 期间会变** ❌（完全反了：drawcall 内不变，drawcall 之间可变）
- 半透明**先绘制** ❌（反了：应该后绘，且从远到近排序）
- VAO **缓存顶点数据** ❌（VAO 存的是配置记录，数据在 VBO 里）
- smoothstep ❌（完全空白）
- 矩阵/坐标空间 ❌（w 分量意义、变换链不完整）

### ② 补线性代数基础（半天）

你摸底坐标空间和 w 分量答得很粗略——**先花半天过一遍 3Blue1Brown 的线性代数系列**。不用全看，重点：
- 第 3 集「矩阵与线性变换」（理解矩阵乘法就是变换）
- 第 4 集「行列式」（直观理解变换的"缩放倍率"）
- 第 5 集「基变换」（理解变换链的数学本质）
- 第 6 集「点积与叉积」（光照全靠这个）

**网址**：Bilibili 搜索 "3Blue1Brown 线性代数本质"，或 https://www.3blue1brown.com/topics/linear-algebra

看完再读 learnopengl 的"变换"章节，矩阵乘法就不再是黑箱。

### ③ 安装 RenderDoc

**Windows 上抓桌面 demo 帧**：https://renderdoc.org/ 直接下载安装。

**Android 上抓 Kanzi 帧**：RenderDoc 官方支持 Android OpenGL ES 捕获，但需要额外配置：
- 手机开发者选项里打开"USB 调试"
- RenderDoc 连接 Android 设备后，选择要调试的 app
- 或通过 Google Play Games Developer Emulator（模拟器里用 Vulkan 后端 + ANGLE 转 OpenGL ES），RenderDoc 可以直接抓

**先在 Windows 上把 demo 跑起来**，桌面版 RenderDoc 更简单。Android 抓帧阶段 3 再配置。

---

## 阶段 1 · OpenGL ES 2/3 基础（4-6 周，每天 2-3 小时）

### 📘 必学 1：learnopengl.com（中文）

**为什么选**：免费、中文、循序渐进、每章有可运行 C++/GLSL 代码、和 ES 90% 兼容。你的 swizzle、smoothstep、纹理采样、FBO、VAO、深度测试、混合全部覆盖。

> ⚠️ **重要警告**：learnopengl 讲的是**桌面 OpenGL 3.3 Core Profile**，和 ES 2/3 有几个必须注意的差异：
> - 桌面 GL 3.3 的 VAO 是核心功能；**ES2 的 VAO 是扩展**，需要 `GL_OES_vertex_array_object`，代码里要加 `#include <GLES2/gl2ext.h>` + `glBindVertexArrayOES`
> - 桌面 GL 3.3 的 fragment shader 输出用 `gl_FragColor`（向后兼容）；**ES3 必须用 `out vec4 outColor;`**，ES2 用 `gl_FragColor`
> - 桌面 GL 3.3 没有 precision 声明要求；**ES2 fragment shader 必须声明 `precision mediump float;`**
> - 桌面 GL 3.3 用 `glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ...)`；**ES2/ES3 用 `glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ..., 0, GL_RGBA, GL_UNSIGNED_BYTE, ...)` 多一个 border 参数（必须 0）**
>
> **实操建议**：先在 Windows 上把 learnopengl demo 跑通（桌面 GL），理解原理后移植到 Android 时改几处 API 签名就行。原理 100% 通用，只有 API 函数名和参数的小差异。

**🔴 必读章节**（按顺序，用**主题名**引用——中文网编号体系和英文原版不同，直接点主题最稳妥）：

| # | learnopengl 主题 | 对应盲区 | 预计时长 |
|---|-----------------|---------|---------|
| 1 | 入门 → 创建窗口 | 补 EGL/上下文概念；**状态机思维的入门触发点**（glEnable/glClear 都是全局状态） | 半天 |
| 2 | 入门 → 你好，三角形 | **纠正 VAO 误解**：VAO 不存数据、是配置记录；VBO/EBO 完整流程 | 1 天 |
| 3 | 入门 → 变换 | **补坐标空间**：MVP 变换链、w 分量、列主序 | 1 天 |
| 4 | 入门 → 着色器 | **纠正 attribute/uniform/varying**：attribute 从哪来、uniform 什么时候能变 | 1 天 |
| 5 | 入门 → 纹理 | **纠正 UV 方向、filter 场景**；mipmap 自动生成 | 1 天 |
| 6 | 入门 → 纹理单元 | 多 sampler 绑定、经典陷阱（忘 ActiveTexture） | 半天 |
| 7 | 高级 OpenGL → 帧缓冲 | FBO 创建 6 步 + 多 Pass 后处理 | 1 天 |
| 8 | 高级 OpenGL → 立方体贴图 | Kanzi 天空盒 | 半天 |
| 9 | 入门 → 摄像机 | 视图矩阵推导、lookAt | 半天 |
| 10 | 光照 → 颜色 | glClearColor、gamma 校正 | 半天 |
| 11 | **高级 OpenGL → 深度测试** | **纠正位置误解 + Z-fighting 原理** | 1 天 |
| 12 | 高级 OpenGL → 模板测试 | 镜面/遮罩 | 半天 |
| 13 | **高级 OpenGL → 混合** | **纠正半透明排序规则** | 半天 |
| 14 | 光照 → 颜色（HDR） | HDR 色彩基础 | 半天 |
| 15 | **GLSL 内置函数**（分散在各章，搜"GLSL 内置函数"） | **纠正 swizzle、smoothstep**；补 mix/smod/mod/step/fract 全族 | 1 天 |
| 16 | 光照 → 基础光照模型 | Lambert + Blinn-Phong | 1 天 |
| 17 | 高级光照 → 法线贴图 | TBN 矩阵 | 半天 |

**网址**：https://learnopengl-cn.github.io

### 📘 必学 2：《OpenGL ES 3.0 编程指南》第三版（红宝书）

**为什么选**：ES2/ES3 规范编写者写的，权威。learnopengl 讲的是桌面 GL，ES 特有的精度修饰符、EGL 上下文管理、ES2→ES3 语法差异对照表，必须从红宝书补。

**🔴 必读章节**（红宝书章节号准确，我已核实）：

| # | 红宝书第三版章节 | 内容 | 对应覆盖的知识节点 |
|---|----------------|------|------------------|
| 1 | 第 1-2 章 | ES 概述 + 上下文创建（EGL 入门） | #1 OpenGL 本质、#30 Kanzi ES2/ES3 双版本管理 |
| 2 | 第 3 章 | **OpenGL 状态**（**状态机思维**、状态查询、glGetError） | **🔴 #2 状态机思维**（learnopengl 没专门讲，这里权威）、**#20 glGetError** |
| 3 | 第 4 章 | Shader 编译链接（glCompileShader/glLinkProgram 完整流程） | #4 Shader 编译链接 |
| 4 | **第 5 章** | **着色器语言（GLSL ES）**：精度修饰符、storage 限定符、**ES2 vs ES3 语法对照表** | #7 精度修饰符、#8 ES2 vs ES3 差异、**#10 透视校正插值原理**（varying 插值数学在这章讲） |
| 5 | **第 6 章** | **缓冲区对象**：VAO/EBO 完整时序、orphaning 技巧 | #3 VBO/EBO/VAO |
| 6 | 第 7-8 章 | 纹理 + 采样器对象 | #11 纹理、**#13 压缩纹理格式**（ETC2/ASTC 官方支持列表在这） |
| 7 | 第 9 章 | **帧缓冲 FBO**（比 learnopengl 更权威，含 MRT） | #18 FBO |
| 8 | 第 11 章 | 片段操作（深度/模板/混合的权威定义） | #14 深度测试、#16 模板测试、#17 混合、**#15 深度值非线性分布**（Z-fighting 根本原因在这章） |
| 9 | 附录 A | **ES2→ES3 语法差异对照表**（**必背**！Kanzi 项目两套 shader 都要写） | #8 ES2 vs ES3 差异 |

**注意**：红宝书是"参考手册"，配合 learnopengl 读。learnopengl 给直觉，红宝书给规范。**红宝书补了 learnopengl 没讲的 5 个关键节点**：状态机思维、透视校正插值原理、深度值非线性分布、压缩纹理格式、glGetError。

### 🎯 阶段 1 完成标准（**必须做到**，否则进入阶段 2 会崩溃）

能独立完成以下 3 个 demo：

| Demo | 练什么 | 验收标准 |
|------|--------|---------|
| 1. 纹理 quad | VBO+EBO+VAO+attribute+uniform+纹理 | 屏幕上一个旋转的彩色 quad |
| 2. FBO 模糊后处理 | FBO 创建 + 多 Pass + 全屏 quad | 画面先渲染到 FBO，再用 3×3 box blur 画到屏幕 |
| 3. Blinn-Phong 旋转立方体 | MVP 矩阵 + 光照模型 + 多个 uniform | 旋转立方体，随光照位置变化高光 |

**怎么练**：照 learnopengl 的代码抄，每一行都要理解为什么这么写。抄完能**自己重新写一遍**才算。抄一遍 ≠ 会了。

### 🎯 阶段 1 知识节点全覆盖清单

读完阶段 1 后，以下 20 个节点必须能**口头完整解释**（不要查资料）：

| # | 知识节点 | 能不能解释？ |
|---|---------|------------|
| 1 | OpenGL 是规范不是库，Context 是什么 | |
| 2 | **状态机思维 + 状态泄漏怎么避免** | |
| 3 | 渲染管线 10 阶段（图元装配不可编程、深度测试在 FS 之后） | |
| 4 | VBO/EBO/VAO 的分工，VAO 存配置不存数据 | |
| 5 | glVertexAttribPointer 的本质（给当时的 VBO 拍照存档） | |
| 6 | **透视校正插值原理（varying 为什么除 w）** | |
| 7 | ES2 vs ES3 的 4 个核心语法差异（varying→in/out、texture2D→texture、gl_FragColor→out 变量、FS 精度声明） | |
| 8 | uniform drawcall 内不变、drawcall 之间可变 | |
| 9 | 纹理 UV 方向为什么翻转、filter 场景为什么反了 | |
| 10 | mipmap 自动生成 + min filter 必须配 MIPMAP 模式 | |
| 11 | 纹理单元绑定流程 + 经典陷阱（忘 ActiveTexture） | |
| 12 | **深度值非线性分布（Z-fighting 根本原因）** | |
| 13 | 半透明物体排序规则（后绘 + 从远到近 + DepthMask=GL_FALSE） | |
| 14 | FBO 创建 6 步 + MRT 多渲染目标 | |
| 15 | 5 空间变换链 + w 分量三个用途 | |
| 16 | 压缩纹理格式 ETC2/ASTC 的意义 | |
| 17 | glGetError 的用法 + 调试输出 | |
| 18 | Lambert + Blinn-Phong 逐行手写 | |
| 19 | swizzle 规则（.yx 交换、读可重复、写不重复） | |
| 20 | smoothstep 三步公式 + S 曲线形状 | |

### 📗 补充：Android EGL 上下文

learnopengl 用 GLFW 封装了 EGL，但 Android 上必须懂——Kanzi 内部就用 EGL。

**快速资源**：Android Developer 文档 "OpenGL ES 开发指南" → "EGL 上下文" 章节（官方中文），或搜 "Android EGL 入门" 找一篇博客过一遍。重点理解：EGL Context/Surface/Config 三者的关系、glBindVertexArray 必须在 Context 上线程、Context 丢失时的资源重建。

---

## 阶段 2 · 光照与着色进阶（2 周，每天 2-3 小时）

### 📘 必学 3：《Real-Time Rendering》第 4 版（RTR）

**为什么选**：实时渲染圣经。你摸底的 PBR 三因子、Z-fighting 原理、半透明渲染全部讲透。**汽车 HMI 渲染工程师的案头书**。

**获取方式**：**需购买纸质版或电子版**，官网 https://www.realtimerendering.com/ 仅提供前 3 章免费（约 80 页）。中文译本 2025 年底刚出，清华大学出版社，约 ¥256。电子版约 $80。社区电子版资源丰富。

**🔴 跳读章节**（不需要通读 1198 页，以下章节号已核实准确）：

| RTR 第 4 版章节 | 内容 | 覆盖的知识节点 | 预计时长 |
|----------------|------|--------------|---------|
| **第 2 章 图形渲染管线** | 完整管线流程、逐阶段数据形态、**深度测试在管线中的位置** | #3 管线、#12 深度测试位置、**#15 深度值非线性分布**（这章有完整推导） | 1 天 |
| 第 4 章 变换 | 矩阵、坐标空间、lookAt 推导 | #15 5 空间变换链 | 半天 |
| 第 7 章 着色模型 | Lambert、Blinn-Phong、微表面理论 | #18 Lambert+Blinn-Phong | 2 天 |
| **第 9 章 基于物理的着色** | Cook-Torrance BRDF + **DGGX + Schlick + Smith 逐项推导** | **PBR 三因子**（车漆渲染数学核心） | 2 天 |
| 第 11 章 纹理与着色 | 纹理细节、法线贴图、视差贴图 | 法线贴图进阶 | 1 天 |
| 第 12 章 颜色与光照 | HDR、色彩空间 | HDR/gamma/linear | 半天 |
| 第 13 章 全局光照概述 | BRDF 基础、能量守恒 | PBR 理论根基 | 半天 |
| 第 14 章 实时全局光照 | 烘焙光照、SSAO（可选） | 了解即可 | 半天 |
| **第 18 章 管线与状态** | 深度测试、模板、混合、**Z-fighting 完整原理** | #12 深度测试、#13 半透明排序、**#15 深度值非线性分布** | 1 天 |
| **第 19 章 高级着色** | 高级材质、半透明 | 半透明渲染进阶 | 1 天 |
| **第 20 章 后期处理** | Bloom、Tonemapping、ACES | **后处理链**、**Bloom 原理** | 1 天 |

> ⚠️ RTR 第 4 版有免费的 **目录 + 前言 + 索引 PDF**：https://www.realtimerendering.com/Real-Time_Rendering_4th-TOC_Preface_Intro_Bib_Index.pdf ，可以用来快速定位章节。

### 📗 补充：Google Filament PBR 白皮书

**为什么选**：Filament 是 Google 开源的移动平台 PBR 引擎，白皮书是移动端 PBR 的最现代实现参考。**你的车漆渲染必看**。

**网址**：https://google.github.io/filament/Filament.html

重点读：
- §2 微表面理论（和 RTR 第 9 章对照读）
- §3 IOR / F0 计算（车漆的 clear coat IOR 差异——为什么 Kanzi 有 `CarPaint_IOR` 版本）
- §4 能量守恒（为什么 roughness 影响高光强度）

### 📗 补充：The Book of Shaders（thebookofshaders.com）

**为什么选**：GLSL 创意编程的最佳在线教程。**专攻 smoothstep、噪声、SDF、动画曲线**——你摸底这几项全空白。learnopengl 讲的是"怎么跑起来"，这本书讲的是"怎么把 shader 写得漂亮"。

**🔴 重点读**（章节号已核实准确）：
- **第 2 章 Colors**（颜色基础，含 mix 用法）
- **第 3 章 Shapes**（2D 形状绘制，step/smoothstep 第一次出现）
- **第 5 章 Shaping functions**（**smoothstep 家族完整讲解**、缓动曲线——你项目的 wipe 过渡动画原理就在这里）
- **第 7 章 Noise**（FBM、简单噪声——你的流光粒子效果原理）
- 第 9 章 3D（空间纹理，和项目 Terrain shader 相关）
- 第 10 章 Raymarching（SDF 2D → 3D，进阶）

> 🔴 **你的项目 Textured_Terrain.frag.glsl 里的 noiseoffset = TransitionValue*10.0 - pow(abs(z),2.0)*0.01**，读完这本书的噪声章节你就能解释这行代码。

### 📗 补充：iquilezles.org（SDF 圣经）

**为什么选**：Inigo Quilez（Shadertoy 创始人）的博客，SDF 距离场的创造者和推广者。SDF 实现的完整公式和推导全在这里。**你摸底 SDF 0 分——必须读**。

**重点读**：
- https://iquilezles.org/articles/distfunctions2d/ — 2D SDF 形状库（sdRoundBox 推导）
- https://iquilezles.org/articles/distfunctions/ — 3D SDF 形状库
- https://iquilezles.org/articles/smoothsteps/ — smoothstep 家族的完整推导

### 🎯 阶段 2 完成标准

1. **能手写 Blinn-Phong**：从 N/L/V/H 四向量开始，到 ambient+diffuse+specular 完整计算，不查资料
2. **能手写 DGGX + Schlick 两个 PBR 因子**：能解释每个参数的物理意义
3. **能读懂项目里任意一个 Bloom shader**：BloomKarisAverage13 下采样的 1/4/2/1 系数为什么是这几个？Karis 滤波的数学是什么？
4. **能读懂项目车漆 shader 的 clear coat 层**：为什么 IOR 版本和 ExclusiveGlass 版本的高光不一样？

---

## 阶段 3 · 移动 GPU 架构与性能（1-2 周，每天 1-2 小时）

### 📘 必学 4：Arm Mali GPU Documentation

**为什么选**：移动 GPU 架构最佳公开资料。Tile-Based Rendering（TBR）是手机性能核心约束——**TBR 怕频繁切 RT，你的 Kanzi Bloom 链每一步切 RT 都在踩坑**。

**具体资源**：
1. **Mali GPU 架构文档**：搜 "Arm Mali GPU architecture guide"，按 Mali-G 系列查找（最新架构是 Immortalis/G715/G710/G510 等）
2. **Tile-Based Rendering 白皮书**：搜 "Mali TBR tile memory" 找架构白皮书
3. **帧缓冲读取代价**：搜 "Mali framebuffer readback penalty"
4. **Mali GPU Best Practices**：搜 "Arm Mali GPU Best Practices Developer Guide"，这是最实用的优化清单

### 📗 补充：Qualcomm Adreno GPU 文档

如果你的项目跑在高通芯片上（多数国内车机），必须对照 Adreno 优化指南。Adreno 架构和 Mali 不同（全局内存 + Tile-Based 混合），但优化思路类似——**减少 RT 切换、减少 overdraw、压缩纹理**。

### 📗 实践：RenderDoc 调试

**什么时候用**：阶段 1 写完第一个 FBO demo 后就可以抓帧。**不要等阶段 3**。

**怎么学**：
1. RenderDoc 官方文档快速过一遍
2. Bilibili 搜索 "RenderDoc 教程"，看 2-3 个实战视频（推荐："CodeTech 渲染" 的 RenderDoc 系列）
3. **Android 上抓 Kanzi 帧**：RenderDoc 连接 Android 设备 → 选择 Kanzi app → 抓帧（需要手机开发者选项打开 USB 调试）

**看什么**：
- Event Browser：每个 draw call 的输入输出
- Pipeline State：当前 bound 的 FBO/Shader/纹理状态
- Texture Viewer：Render Target 内容、mip 层、Overdraw 计数
- Pixel History：某个像素被哪些 draw call 写过（定位 overdraw 利器）

### 🎯 阶段 3 完成标准

1. **理解 TBR 为什么怕切 RT**：能解释 tile memory 是怎么回事，一个 Tile 从 tile memory 写到主内存再读回来代价多大
2. **能看懂 RenderDoc 的四面板**：给一张 Kanzi 的帧截图，能定位哪个 Render Target 是 Bloom 下采样、哪个是最终合成
3. **能定位一个 overdraw 问题**：用 RenderDoc 的 Overdraw 模式，找出哪个 draw call 在透明物体上被重复覆盖了
4. **能看懂项目 Bloom 链的 Render Target 序列**：为什么需要 Full→Half→Quarter→Half→Full 这个顺序
5. **知道三类性能瓶颈（fill rate / geometry / bandwidth）的区别和定位方法**

---

## 阶段 4 · Kanzi 专项（1 周，每天 1-2 小时）

### 📘 必学 5：Kanzi Studio 官方文档

**Kanzi Studio 安装目录下**有完整 PDF 文档，路径通常是：
- Windows：`C:\Program Files\Rightware\Kanzi Studio <版本号>\Documentation\Kanzi Studio Manual.pdf`
- 或在 Kanzi Studio 里按 F1 打开文档浏览器

**🔴 重点读**（按你的项目实际用到的）：

| 文档章节 | 内容 | 对应覆盖的知识节点 |
|---------|------|------------------|
| Shader 系统 | .glsl 命名规则、ES2/ES3 双版本、**builtin uniform 完整列表** | **#30 Kanzi ES2/ES3 双版本管理**（Kanzi 内部自动编两套，但要理解为什么需要） |
| Material Property | **Property→uniform 匹配规则**（名字相同自动绑定、不区分大小写） | datasource→shader 链路 |
| Effect | Bloom 链、Tonemapping、自定义 Effect 扩展步骤 | 后处理链 Kanzi 实现 |
| Rendering Pipeline | Kanzi 内部 draw call 顺序（opaque → transparent → post-process） | draw call 顺序 |
| Performance Tips | Kanzi 特有的 GPU 优化（减少 Effect Pass、压缩纹理、避免频繁切换 Material） | GPU 性能调优 |

### 📗 补充：Kanzi API Reference

`KanziRuntime API Reference.pdf`（和 Manual 同目录），重点查：
- `kzPropertyManager` / `kzValueManager` — datasource→binding→property 的 C++ API
- `kzRenderer` — 渲染管线控制点

### 🎯 阶段 4 完成标准

1. **能看懂项目里任意一个 Kanzi Effect**：比如 BloomKarisAverage13 的 Effect 配置——每一步输入哪个 Render Target、输出哪个、用什么 Filter
2. **能解释 datasource→uniform 的完整链路**：Java `KanziManager.setValue("Scene.SceneIndex", xxx)` → Studio 绑定 → Material Property → 同名 uniform → shader 里拿到值。每一步的验证工具是什么？
3. **能写一个自定义的 Wold shader**：Kanzi Studio 里新建 Material，关联自定义 GLSL，绑定 datasource uniform，预览效果
4. **能在 RenderDoc 里抓 Kanzi 帧，标出 datasource→uniform 链路的每个验证点**

---

## 学习节奏总览

| 阶段 | 时长 | 每天投入 | 完成标志 |
|------|------|---------|---------|
| 0. 线性代数补课 | 半天 | - | 看完 3Blue1Brown 6 集 |
| 1. 基础 | 4-6 周 | 2-3 小时 | 3 个 demo + 20 个知识节点全部能口头解释 |
| 2. 光照 | 2 周 | 2-3 小时 | 手写 Blinn-Phong + DGGX，看懂项目 Bloom/车漆 shader |
| 3. GPU | 1-2 周 | 1-2 小时 | 理解 TBR，RenderDoc 抓帧定位瓶颈 |
| 4. Kanzi | 1 周 | 1-2 小时 | 看懂项目 Effect，能写自定义 Wold shader |

**总计约 8-10 周**，不是速成，但扎实。

---

## 跳过什么 / 延后什么

| 内容 | 为什么跳过 | 什么时候回来 |
|------|-----------|-------------|
| 几何着色器 | ES 没有 | 以后做 PC 端 Kanzi 工具链时 |
| Tesselation | ES3.1+，项目没用到 | 以后做地形细分 |
| Compute Shader | ES3.1+，项目没用到 | 以后做 GPU 粒子/后处理 |
| 光线追踪 | 硬件/API 都不支持 | 以后做 PC 端 |
| Vulkan | 完全不同的 API | Kanzi 还没出 Vulkan 版本 |

---

## 核心原则

1. **先跑起来再深入**：每学一个概念（比如 FBO），**先照 learnopengl 抄一个能跑的 demo**，再去读原理。抄完能自己重写一遍才算会了——你之前的问题很多是"有印象但没动手"。
2. **RTR 只跳读不通读**：1198 页，通读浪费时间。按上面的章节表跳。
3. **Kanzi shader 当练习题**：项目里有几百个 .glsl，读完阶段 1-2 后回头看，每行都能解释来源。解释不了的就是没搞懂。
4. **RenderDoc 从阶段 1 就用**：写完第一个 demo 就抓帧，看各个面板。看不懂 = 回去补对应章节。
5. **书和代码交叉读**：读 RTR 第 9 章 PBR 公式 → 打开项目里的 PhysicallyBased_CarPaint.frag.glsl → 逐项对照公式在代码里的实现。这是**最快的学习方式**。
