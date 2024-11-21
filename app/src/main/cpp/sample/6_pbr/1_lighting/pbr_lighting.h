//
// Created by ts on 2024/11/20.
//

/**
 * 微平面模型：在微观尺度上任何平面都可以被微平面(Microfacets)的细小镜面来进行描述。
 * 一个平面越是粗糙，这个平面上的微平面的排列就越混乱。入射光线更趋向于向完全不同的方向发散(Scatter)开来，进而产生出分布范围更广泛的镜面反射。
 *
 * 粗糙度(Roughness)参数：统计学的方法来估计微平面的粗糙程度。
 *
 * 能量守恒：出射光线的能量永远不能超过入射光线的能量（发光面除外）。
 *
 * 反射率方程：Lo(p,ωo)=∫Ωfr(p,ωi,ωo)Li(p,ωi)n⋅ωidωi
 * 反射率公式计算了点p在ωo方向上被反射出来的辐射率Lo(p,ωo)的总和。或者换句话说：Lo表示了从ωo方向上观察，光线投射到点p上反射出来的辐照度。
 *
 * 辐射率(Radiance)：在这里用 L 来表示。辐射率被用来量化来自单一方向上的光线的大小或者强度。L=d2Φ/dAdωcosθ
 * 辐射率方程很有用，因为它把大部分我们感兴趣的物理量都包含了进去。如果我们把立体角ω和面积A看作是无穷小的，那么我们就能用辐射率来表示单束光线穿过空间中的一个点的通量。
 * 把立体角ω转变为方向向量ω然后把面A转换为点p。这样我们就能直接在我们的着色器中使用辐射率来计算单束光线对每个片段的作用了。
 *
 * 辐射通量：辐射通量 Φ 表示的是一个光源所输出的能量，以瓦特为单位。
 *
 * 立体角(Solid Angle)：立体角用 ω 表示，它可以为我们描述投射到单位球体上的一个截面的大小或者面积。
 *
 * 辐射强度：辐射强度(Radiant Intensity)表示的是在单位球面上，一个光源向每单位立体角所投送的辐射通量。
 *
 * 辐射照度/辐照度：所有投射到点 p 上的光线的总和。
 *
 * 半球领域(Hemisphere)：一个以点p为球心的半球领域，一个半球领域(Hemisphere)可以描述为以平面法线n为轴所环绕的半个球体。
 *
 * 黎曼和(Riemann sum) ：在半球领域Ω中按一定的步长将反射率方程分散求解，然后再按照步长大小将所得到的结果平均化。
 *
 * 双向反射分布函数(Bidirectional Reflective Distribution Function) ，BRDF可以近似的求出每束光线对一个给定了
 * 材质属性的平面上最终反射出来的光线所作出的贡献程度。
 *
 * Cook-Torrance BRDF模型兼有漫反射和镜面反射两个部分：fr=kdflambert+ksfcook−torrance
 * kd是入射光线中被折射部分的能量所占的比率，而ks是被反射部分的比率。
 * BRDF的左侧表示的是漫反射部分，这里用flambert来表示。它被称为Lambertian漫反射，用如下的公式来表示：flambert=c/π
 * c 表示表面颜色，除以π是为了对漫反射光进行标准化，因为前面含有BRDF的积分方程是受π影响的。
 *
 * BRDF的镜面反射部分要稍微更高级一些，它的形式如下所示：fcook−torrance=DFG/4(ωo⋅n)(ωi⋅n)
 * 字母D，F与G分别代表着一种类型的函数，各个函数分别用来近似的计算出表面反射特性的一个特定部分：
 * 法线分布函数(Normal Distribution Function)：估算在受到表面粗糙度的影响下，朝向方向与半程向量一致的微平面的数量。这是用来估算微平面的主要函数。
 * 几何函数(Geometry Function)：描述了微平面自成阴影的属性。当一个平面相对比较粗糙的时候，平面表面上的微平面有可能挡住其他的微平面从而减少表面所反射的光线。
 * 菲涅尔方程(Fresnel Rquation)：在不同的表面角下表面所反射的光线所占的比率。
 *
 * 编写PBR材质:
 * 反照率(Albedo)纹理:为每一个金属的纹素(Texel)（纹理像素）指定表面颜色或者基础反射率。
 * 法线：法线贴图纹理和我们之前在法线贴图教程中所使用的贴图是完全一样的。
 * 金属度：金属(Metallic)贴图逐个纹素的指定该纹素是不是金属质地的。
 * 粗糙度：粗糙度(Roughness)贴图可以以纹素为单位指定某个表面有多粗糙。
 * AO：环境光遮蔽(Ambient Occlusion)贴图或者说AO贴图为表面和周围潜在的几何图形指定了一个额外的阴影因子。
 */

#ifndef LEARNOPENGL_PBR_LIGHTING_H
#define LEARNOPENGL_PBR_LIGHTING_H

#include "GLBaseSample.h"

class pbr_lighting : public GLBaseSample {

public:

    pbr_lighting() = default;

    virtual ~pbr_lighting() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint sphereVAO;
    unsigned int indexCount;
    // lights
    // ------
    glm::vec3 lightPositions[4];
    glm::vec3 lightColors[4];
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;
    glm::mat4 projection;

    void renderSphere();
};

#endif //LEARNOPENGL_PBR_LIGHTING_H
