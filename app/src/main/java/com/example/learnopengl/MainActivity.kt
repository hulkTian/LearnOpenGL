package com.example.learnopengl

import android.content.Intent
import android.opengl.GLSurfaceView
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.SparseArray
import android.util.SparseIntArray
import android.view.View
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import com.example.learnopengl.adapter.GLRecyclerAdapter
import com.example.learnopengl.databinding.ActivityMainBinding
import java.util.HashMap

class MainActivity : AppCompatActivity() {

    private val data: MutableList<Map<String, Any?>> = ArrayList()
    private val typeMapping = SparseArray<UIConfig>()
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        initData()

        initRecyclerView()
    }

    private fun initRecyclerView() {
        val layoutManager = LinearLayoutManager(this)
        layoutManager.orientation = LinearLayoutManager.VERTICAL
        binding.recyclerView.layoutManager = layoutManager

        binding.recyclerView.addItemDecoration(
            DividerItemDecoration(
                this,
                DividerItemDecoration.VERTICAL
            )
        )

        val adapter = GLRecyclerAdapter(data, this)
        binding.recyclerView.adapter = adapter
        adapter.setOnItemClickListener { position ->
            val launchIntent = Intent(this, NativeRenderActivity::class.java)
            launchIntent.putExtra(IMyNativeRendererType.RENDER_TYPE, typeMapping[position])
            startActivity(launchIntent)
        }
    }

    //初始化数据
    private fun initData() {
        var i = 0
        addItem(
            i++, R.mipmap.vao_vbo,
            "绘制三角形",
            "第一个渲染例子，展示了VAO和VBO的使用流程；着色器代码的编写、加载和编译流程；以及渲染一个三角形的简单流程。",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_VAO_VBO)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.ebo,
            "绘制矩形",
            "展示如何使用VAO、VBO和IBO绘制矩形",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_EBO)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.vao_vbo_exercise_1,
            "绘制两个三角形",
            "使用glDrawArrays绘制两个相连的三角形，按照默认顺序绘制多个顶点",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_VAO_VBO_EXERCISE_1)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.vao_vbo_exercise_1,
            "绘制两个三角形",
            "使用不同的VAO和VBO绘制两个三角形，理解VAO的意义",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_VAO_VBO_EXERCISE_2)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.triangle,
            "着色器语言GLSL",
            "使用Uniform在片段着色器声明一个变量，实现一个颜色会变化的三角形",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_UNIFORM)
                .setShowClickLayout(View.GONE)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.triangle6,
            "纹理",
            "使用图片生成纹理，了解纹理从生成到使用的过程",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_TEXTURE_WARP)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.texture_unit,
            "纹理单元",
            "通过纹理单元让多个纹理显示在一个图形上",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_TEXTURE_MIPMAP)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.texture_unit,
            "纹理-练习1",
            "调整纹理坐标让纹理反转显示",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_TEXTURE_EXERCISE_1)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.texture_unit,
            "纹理-练习2",
            "设置环绕方式让纹理重复显示",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_TEXTURE_EXERCISE_2)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.texture_unit,
            "纹理-练习3",
            "放大纹理使用GL_NEAREST",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_TEXTURE_EXERCISE_3)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.texture_unit,
            "纹理-练习4",
            "使用uniform控制纹理混合比例",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_TEXTURE_EXERCISE_4)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.transform,
            "变换",
            "使用变换矩阵实现物体的缩放、位移和旋转",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_MAT)
                .setShowClickLayout(View.GONE)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.coordinate,
            "坐标系统",
            "使用矩阵构建坐标系，实现3D效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COORDINATE)
                .setShowClickLayout(View.GONE)
                .build()
        )
        addItem(
            i++, R.mipmap.coordinate,
            "坐标系统-练习1",
            "控制foV和aspect-ratio观察3D场景",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COORDINATE_EXERCISE_1)
                .setShowClickLayout(View.GONE)
                .setShowSeekBar1(View.VISIBLE).setTitle1("foV").build()
        )
        addItem(
            i++, R.mipmap.coordinate,
            "坐标系统-练习2",
            "移动观察矩阵位置观察3D场景",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COORDINATE_EXERCISE_2)
                .setShowClickLayout(View.GONE)
                .setShowSeekBar1(View.VISIBLE).setTitle1("foV").build()
        )
        addItem(
            i++, R.mipmap.coordinate,
            "坐标系统-练习3",
            "使用模型矩阵旋转特定的物体",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COORDINATE_EXERCISE_3)
                .setShowClickLayout(View.GONE)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.coordinate,
            "摄像机",
            "使用摄像机矩阵，实现3D空间观察视角",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CAMERA)
                .setShowClickLayout(View.GONE)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.coordinate,
            "移动摄像机位置",
            "使用事件改变摄像机位置，包括按钮点击和手指滑动",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CAMERA_AUTO_MOVE)
                .setShowSeekBar1(View.VISIBLE).setTitle1("foV").build()
        )
        addItem(
            i++, R.mipmap.color_light,
            "创建一个光照场景",
            "用3D立方体创建一个基本的光源",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COLORS).build()
        )
        addItem(
            i++, R.mipmap.color_light_view,
            "冯氏光照效果",
            "在视图空间中计算冯氏光照效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW).build()
        )
        addItem(
            i++, R.mipmap.color_light_view,
            "冯氏光照效果-练习1",
            "移动光源位置观察光照效果变化",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW_EXERCISE_1)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.color_light_view,
            "冯氏光照效果-练习2",
            "使用不同的环境光、漫反射和镜面反射系数观察光照效果变化",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW_EXERCISE_2)
                .setShowSeekBar1(View.VISIBLE).setTitle1("环境光因子")
                .setShowSeekBar2(View.VISIBLE).setTitle2("漫反射因子")
                .setShowSeekBar3(View.VISIBLE).setTitle3("镜面强度因子").build()
        )
        addItem(
            i++, R.mipmap.color_light_view,
            "冯氏光照效果-练习3",
            "在观察空间中计算冯氏光照效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW_EXERCISE_3)
                .setShowSeekBar1(View.VISIBLE).setTitle1("环境光因子")
                .setShowSeekBar2(View.VISIBLE).setTitle2("漫反射因子")
                .setShowSeekBar3(View.VISIBLE).setTitle3("镜面强度因子").build()
        )
        addItem(
            i++, R.mipmap.color_light_view,
            "冯氏光照效果-练习4",
            "Gouraud 光照效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW_EXERCISE_4)
                .setShowSeekBar1(View.VISIBLE).setTitle1("环境光因子")
                .setShowSeekBar2(View.VISIBLE).setTitle2("漫反射因子")
                .setShowSeekBar3(View.VISIBLE).setTitle3("镜面强度因子").build()
        )
        addItem(
            i++, R.mipmap.material,
            "材质",
            "通过材质和光源颜色细微控制物体表面颜色",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_COLORS_MATERIAL)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.lighting_diffuse,
            "光照贴图",
            "使用纹理贴图代替材质中的单色分量",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_MAPS_DIFFUSE).build()
        )
        addItem(
            i++, R.mipmap.light_caster,
            "投光物",
            "在立方体箱子中使用平行光效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_CASTERS_DIRECTIONAL).build()
        )
        addItem(
            i++, R.mipmap.lights_merge,
            "多光源",
            "在立方体箱子中使用多种光源构建氛围效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_MERGE).build()
        )
        addItem(
            i++, R.mipmap.model_loading,
            "模型",
            "使用 Assimp 加载模型",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_MODEL_LOADING).build()
        )
        addItem(
            i++, R.mipmap.depth_testing,
            "深度测试",
            "验证深度测试配置参数效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_DEPTH_TESTING).build()
        )
        addItem(
            i++, R.mipmap.depth_display,
            "模板测试可视化",
            "将深度值作为颜色输出",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_DEPTH_DISPLAY).build()
        )
        addItem(
            i++, R.mipmap.depth_testing,
            "模板测试",
            "使用模板测试实现边框效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_STENCIL_TESTING).build()
        )

        addItem(
            i++, R.mipmap.blending_discard,
            "混合",
            "使用带透明度通道的图片,丢弃透明的片段",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_BLENDING_DISCARD).build()
        )
        addItem(
            i++, R.mipmap.blending_discard,
            "面剔除",
            "启用面剔除功能，提升渲染立方体的性能",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CULL_FACE).build()
        )
        addItem(
            i++, R.mipmap.frame_buffers,
            "帧缓冲",
            "将颜色缓冲、深度缓冲、模板缓冲结合存放到帧缓冲中，并作为一张纹理进行渲染，方便进行后期处理",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_FRAME_BUFFERS).build()
        )
        addItem(
            i++, R.mipmap.frame_buffers_exercise,
            "帧缓冲练习：后视镜效果",
            "使用帧缓冲实现后视镜效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_FRAME_BUFFERS_EXERCISE).build()
        )
        addItem(
            i++, R.mipmap.cube_map,
            "立方体贴图",
            "使用立方体贴图实现天空盒子效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS).build()
        )
        addItem(
            i++, R.mipmap.cube_map_reflection_box,
            "环境映射——反射箱子",
            "反射天空盒子的颜色",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFLECTION1).build()
        )
        addItem(
            i++, R.mipmap.cube_map_reflection_mode,
            "环境映射——反射模型",
            "使用模型反射天空盒子颜色",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFLECTION2).build()
        )
        addItem(
            i++, R.mipmap.cube_map_refraction,
            "环境映射——折射",
            "折射天空盒子颜色",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFRACTION).build()
        )
        addItem(
            i++, R.mipmap.cube_map_reflection_exercise,
            "环境映射——练习",
            "天空盒子中展示有反射贴图的模型",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_EXERCISE).build()
        )
        addItem(
            i++, R.mipmap.ubo,
            "Uniform Buffer Object",
            "在多个着色器中使用Uniform 缓冲对象存储的顶点数据",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_CUBE_UNIFORM_BUFFER)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.ubo,
            "几何着色器",
            "几何着色器的基本使用方式",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_GEOMETRY_SHADER)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.instancing,
            "实例化",
            "使用实例化将数据一次传递给GPU",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_INSTANCING).build()
        )
        addItem(
            i++, R.mipmap.instancing_asteroids,
            "实例化-行星带",
            "使用实例化实现行星带效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_INSTANCING_ASTEROIDS).build()
        )
        addItem(
            i++, R.mipmap.instancing_asteroids,
            "抗锯齿-MSAA",
            "使用多重采样缓冲帧实现抗锯齿",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_ANIT_ALIASING).build()
        )
        addItem(
            i++, R.mipmap.advanced_lighting,
            "高级光照-Blinn-Phong",
            "使用Blinn-Phong 优化镜面反射",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_ADVANCED_LIGHTING).build()
        )
        addItem(
            i++, R.mipmap.advanced_lighting_gamma_correction,
            "高级光照-Gamma Correction",
            "Gamma 校正使光照颜色更自然",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_ADVANCED_LIGHTING_GAMMA_CORRECTED)
                .setShowBooleanButton(View.VISIBLE).build()
        )
        addItem(
            i++, R.mipmap.shadow_mapping_depth,
            "高级光照-深度贴图",
            "使用帧缓冲获取场景深度贴图",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_SHADOW_MAPPING_DEPTH)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.shadow_mapping_base,
            "高级光照-渲染阴影",
            "使用帧缓冲获取场景深度贴图，在正常的场景中使用深度值渲染阴影",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_SHADOW_MAPPING_BASE).build()
        )
        addItem(
            i++, R.mipmap.normal_mapping,
            "高级光照-法线贴图",
            "通过法线贴图让平面在光照下更加真实",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_NORMAL_MAPPING)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.parallax_mapping,
            "高级光照-视差贴图",
            "通过视差贴图让平面产生高度差效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_PARALLAX_MAPPING)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.steep_parallax_mapping,
            "高级光照-陡峭视差映射",
            "解决普通视差贴图计算中存在的陡峭的高度导致的不真实的问题",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_STEEP_PARALLAX_MAPPING)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.steep_parallax_mapping,
            "高级光照-陡峭视差映射",
            "解决普通视差贴图计算中存在的陡峭的高度导致的不真实的问题",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_STEEP_PARALLAX_MAPPING)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.parallax_occlusion_mapping,
            "高级光照-视差遮蔽映射",
            "在陡峭视差映射基础上增加偏移的纹理坐标的线性插值计算，使高度效果更柔和",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_BLENDING_DISCARD)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.hdr,
            "高级光照-HDR",
            "使用高精度颜色缓冲获取更丰富的色彩还原",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_HDR)
                .setShowBooleanButton(View.VISIBLE).setShowSeekBar1(View.VISIBLE).setTitle1("foV").build()
        )
        addItem(
            i++, R.mipmap.bloom,
            "高级光照-泛光",
            "使用高斯模糊实现泛光效果",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_BLOOM)
                .setShowBooleanButton(View.VISIBLE).setShowSeekBar1(View.VISIBLE).setTitle1("foV").build()
        )
        addItem(
            i++, R.mipmap.deferred_shading,
            "高级光照-延迟着色法",
            "多光源场景使用延迟着色法优化显示性能",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_DEFERRED_SHADING).build()
        )
        addItem(
            i++, R.mipmap.ssao,
            "高级光照-SSAO",
            "环境光遮蔽使场景更真实",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_SSAO).build()
        )
        addItem(
            i++, R.mipmap.pbr_lighting,
            "高级光照-PBR",
            "直接光显示PBR",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_PBR_LIGHTING).build()
        )
        addItem(
            i++, R.mipmap.pbr_lighting_textured,
            "高级光照-PBR纹理材质",
            "PBR纹理材质",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_PBR_LIGHTING_TEXTURED).build()
        )
        addItem(
            i++, R.mipmap.pbr_hdr_conversion,
            "高级光照-PBR和HDR",
            "HDR 环境贴图转换为立方体贴图，并将直接光照的PBR球体显示在立方体贴图中",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_IRRADIANCE_CONVERSION).build()
        )
        addItem(
            i++, R.mipmap.pbr_ibl,
            "高级光照-PBR和IBL漫反射",
            "IBL贴图作为环境光照渲染PBR球体",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_IRRADIANCE).build()
        )
        addItem(
            i++, R.mipmap.pbr_ibl,
            "高级光照-PBR和IBL镜面反射",
            "IBL贴图作为环境光照和镜面反射光照渲染PBR球体",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_SPECULAR).build()
        )
        addItem(
            i++, R.mipmap.pbr_ibl,
            "高级光照-PBR和IBL镜面反射纹理",
            "IBL贴图作为环境光照和镜面反射光照渲染PBR球体",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_SPECULAR_TEXTURED).build()
        )
        addItem(
            i++, R.mipmap.text_rendering,
            "调试OpenGL",
            "打印OpenGl 错误信息",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_DEBUGGING)
                .setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.text_rendering,
            "文本渲染",
            "使用FreeType库渲染文本",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_TEXT_RENDERING).setShowClickLayout(View.GONE).build()
        )
        addItem(
            i++, R.mipmap.break_out,
            "Breakout 2D游戏",
            "结合所有OpenGL技术实现一个2D游戏",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_BREAK_OUT)
                .setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY).build()
        )
        addItem(
            i++, R.mipmap.weighted_blended,
            "不依赖顺序的混合技术（OIT）",
            "",
            BuildUI().setType(IMyNativeRendererType.SAMPLE_TYPE_WEIGHTED_BLENDED_OIT).build()
        )
    }

    private fun addItem(index: Int, redId: Int, title: String, subtitle: String, type: UIConfig) {
        val item: MutableMap<String, Any?> = HashMap()
        item[ITEM_IMAGE] = redId
        item[ITEM_TITLE] = title
        item[ITEM_SUBTITLE] = subtitle
        data.add(item)
        //记录item需要展示的内容类型，点击对应item后，根据这个类型展示相应的内容
        typeMapping.put(index, type)
    }

    companion object {
        const val ITEM_IMAGE = "item_image"
        const val ITEM_TITLE = "item_title"
        const val ITEM_SUBTITLE = "item_subtitle"
    }
}