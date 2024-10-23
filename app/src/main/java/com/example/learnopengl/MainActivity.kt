package com.example.learnopengl

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.SparseIntArray
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import com.example.learnopengl.adapter.GLRecyclerAdapter
import com.example.learnopengl.databinding.ActivityMainBinding
import java.util.HashMap

class MainActivity : AppCompatActivity() {

    private val data: MutableList<Map<String, Any?>> = ArrayList()
    private val typeMapping = SparseIntArray()
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
            i++, R.mipmap.triangle,
            "绘制三角形",
            "通过绘制一个三角形，理解VAO、VBO、顶点属性指针、着色器代码编译过程、着色器程序对象和图形的渲染过程。",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE
        )
        addItem(
            i++, R.mipmap.rectangle,
            "绘制矩形",
            "展示如何使用VAO、VBO和IBO绘制矩形",
            IMyNativeRendererType.SAMPLE_TYPE_RECTANGLE
        )
        addItem(
            i++, R.mipmap.triangle2,
            "绘制彩色三角形",
            "在着色器代码中定义两个属性，一个位置属性，一个颜色属性，并通过CPU代码把位置数据和颜色数据设置给两个属性",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE2
        )
        addItem(
            i++, R.mipmap.triangle3,
            "绘制两个三角形",
            "使用glDrawArrays绘制两个相连的三角形，按照默认顺序绘制多个顶点",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE3
        )
        addItem(
            i++, R.mipmap.triangle3,
            "绘制两个三角形",
            "使用不同的VAO和VBO绘制两个三角形，理解VAO的意义",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE4
        )
        addItem(
            i++, R.mipmap.triangle,
            "着色器语言GLSL",
            "使用Uniform在片段着色器声明一个变量，实现一个颜色会变化的三角形",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE5
        )
        addItem(
            i++, R.mipmap.triangle6,
            "纹理",
            "使用图片生成纹理，了解纹理冲生成到使用的过程",
            IMyNativeRendererType.SAMPLE_TYPE_TEXTURE
        )
        addItem(
            i++, R.mipmap.texture_unit,
            "纹理单元",
            "通过纹理单元让多个纹理显示在一个图形上",
            IMyNativeRendererType.SAMPLE_TYPE_TEXTURE2
        )
        addItem(
            i++, R.mipmap.transform,
            "变换",
            "使用变换矩阵实现物体的缩放、位移和旋转",
            IMyNativeRendererType.SAMPLE_TYPE_MAT
        )
        addItem(
            i++, R.mipmap.coordinate,
            "坐标系统",
            "使用矩阵构建坐标系，实现3D效果",
            IMyNativeRendererType.SAMPLE_TYPE_COORDINATE
        )
        addItem(
            i++, R.mipmap.coordinate,
            "摄像机",
            "使用摄像机矩阵，实现3D空间观察视角",
            IMyNativeRendererType.SAMPLE_TYPE_CAMERA
        )
        addItem(
            i++, R.mipmap.coordinate,
            "移动摄像机位置",
            "使用事件改变摄像机位置，包括按钮点击和手指滑动",
            IMyNativeRendererType.SAMPLE_TYPE_CAMERA_AUTO_MOVE
        )
        addItem(
            i++, R.mipmap.color_light,
            "创建一个光照场景",
            "用3D立方体创建一个基本的光源",
            IMyNativeRendererType.SAMPLE_TYPE_COLORS
        )
        addItem(
            i++, R.mipmap.color_light_view,
            "冯氏光照效果",
            "在视图空间中计算冯氏光照效果",
            IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW
        )
        addItem(
            i++, R.mipmap.material,
            "材质",
            "通过材质和光源颜色细微控制物体表面颜色",
            IMyNativeRendererType.SAMPLE_TYPE_COLORS_MATERIAL
        )
        addItem(
            i++, R.mipmap.lighting_diffuse,
            "光照贴图",
            "使用纹理贴图代替材质中的单色分量",
            IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_MAPS_DIFFUSE
        )
        addItem(
            i++, R.mipmap.light_caster,
            "投光物",
            "在立方体箱子中使用平行光效果",
            IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_CASTERS_DIRECTIONAL
        )
        addItem(
            i++, R.mipmap.lights_merge,
            "多光源",
            "在立方体箱子中使用多种光源构建氛围效果",
            IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_MERGE
        )
        addItem(
            i++, R.mipmap.model_loading,
            "模型",
            "使用 Assimp 加载模型",
            IMyNativeRendererType.SAMPLE_TYPE_MODEL_LOADING
        )
        addItem(
            i++, R.mipmap.depth_testing,
            "深度测试",
            "验证深度测试配置参数效果",
            IMyNativeRendererType.SAMPLE_TYPE_DEPTH_TESTING
        )
        addItem(
            i++, R.mipmap.depth_testing,
            "模板测试",
            "使用模板测试实现边框效果",
            IMyNativeRendererType.SAMPLE_TYPE_STENCIL_TESTING
        )
        addItem(
            i++, R.mipmap.blending_discard,
            "混合",
            "使用带透明度通道的图片,丢弃透明的片段",
            IMyNativeRendererType.SAMPLE_TYPE_BLENDING_DISCARD
        )
        addItem(
            i++, R.mipmap.blending_discard,
            "面剔除",
            "启用面剔除功能，提升渲染立方体的性能",
            IMyNativeRendererType.SAMPLE_TYPE_CULL_FACE
        )
        addItem(
            i++, R.mipmap.frame_buffers,
            "帧缓冲",
            "将颜色缓冲、深度缓冲、模板缓冲结合存放到帧缓冲中，并作为一张纹理进行渲染，方便进行后期处理",
            IMyNativeRendererType.SAMPLE_TYPE_FRAME_BUFFERS
        )
        addItem(
            i++, R.mipmap.frame_buffers_exercise,
            "帧缓冲练习：后视镜效果",
            "使用帧缓冲实现后视镜效果",
            IMyNativeRendererType.SAMPLE_TYPE_FRAME_BUFFERS_EXERCISE
        )
        addItem(
            i++, R.mipmap.cube_map,
            "立方体贴图",
            "使用立方体贴图实现天空盒子效果",
            IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS
        )
        addItem(
            i++, R.mipmap.cube_map_reflection_box,
            "环境映射——反射箱子",
            "反射天空盒子的颜色",
            IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFLECTION1
        )
        addItem(
            i++, R.mipmap.cube_map_reflection_mode,
            "环境映射——反射模型",
            "使用模型反射天空盒子颜色",
            IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFLECTION2
        )
        addItem(
            i++, R.mipmap.cube_map_refraction,
            "环境映射——折射",
            "折射天空盒子颜色",
            IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFRACTION
        )
        addItem(
            i++, R.mipmap.cube_map_reflection_exercise,
            "环境映射——练习",
            "天空盒子中展示有反射贴图的模型",
            IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_EXERCISE
        )
        addItem(
            i++, R.mipmap.ubo,
            "Uniform Buffer Object",
            "在多个着色器中使用Uniform 缓冲对象存储的顶点数据",
            IMyNativeRendererType.SAMPLE_TYPE_CUBE_UNIFORM_BUFFER
        )
        addItem(
            i++, R.mipmap.ubo,
            "几何着色器",
            "几何着色器的基本使用方式",
            IMyNativeRendererType.SAMPLE_TYPE_GEOMETRY_SHADER
        )
        addItem(
            i++, R.mipmap.instancing,
            "实例化",
            "使用实例化将数据一次传递给GPU",
            IMyNativeRendererType.SAMPLE_TYPE_INSTANCING
        )
        addItem(
            i++, R.mipmap.instancing_asteroids,
            "实例化-行星带",
            "使用实例化实现行星带效果",
            IMyNativeRendererType.SAMPLE_TYPE_INSTANCING_ASTEROIDS
        )
        addItem(
            i++, R.mipmap.instancing_asteroids,
            "抗锯齿-MSAA",
            "使用多重采样缓冲帧实现抗锯齿",
            IMyNativeRendererType.SAMPLE_TYPE_ANIT_ALIASING
        )
        addItem(
            i++, R.mipmap.advanced_lighting,
            "高级光照-Blinn-Phong",
            "使用Blinn-Phong 优化镜面反射",
            IMyNativeRendererType.SAMPLE_TYPE_ADVANCED_LIGHTING
        )
        addItem(
            i++, R.mipmap.advanced_lighting_gamma_correction,
            "高级光照-Gamma Correction",
            "Gamma 校正使光照颜色更自然",
            IMyNativeRendererType.SAMPLE_TYPE_ADVANCED_LIGHTING_GAMMA_CORRECTED
        )
    }

    private fun addItem(index: Int, redId: Int, title: String, subtitle: String, type: Int) {
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