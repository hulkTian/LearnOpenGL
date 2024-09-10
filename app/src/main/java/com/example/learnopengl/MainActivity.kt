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
            i++,
            R.mipmap.triangle,
            "绘制三角形",
            "通过绘制一个三角形，理解VAO、VBO、顶点属性指针、着色器代码编译过程、着色器程序对象和图形的渲染过程。",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE
        )
        addItem(
            i++,
            R.mipmap.rectangle,
            "绘制矩形",
            "展示如何使用VAO、VBO和IBO绘制矩形",
            IMyNativeRendererType.SAMPLE_TYPE_RECTANGLE
        )
        addItem(
            i++,
            R.mipmap.triangle2,
            "绘制彩色三角形",
            "在着色器代码中定义两个属性，一个位置属性，一个颜色属性，并通过CPU代码把位置数据和颜色数据设置给两个属性",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE2
        )
        addItem(
            i++,
            R.mipmap.triangle3,
            "绘制两个三角形",
            "使用glDrawArrays绘制两个相连的三角形，按照默认顺序绘制多个顶点",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE3
        )
        addItem(
            i++,
            R.mipmap.triangle3,
            "绘制两个三角形",
            "使用不同的VAO和VBO绘制两个三角形，理解VAO的意义",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE4
        )
        addItem(
            i++,
            R.mipmap.triangle,
            "着色器语言GLSL",
            "使用Uniform在片段着色器声明一个变量，实现一个颜色会变化的三角形",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE5
        )
        addItem(
            i++,
            R.mipmap.triangle6,
            "纹理",
            "使用图片生成纹理，了解纹理冲生成到使用的过程",
            IMyNativeRendererType.SAMPLE_TYPE_TEXTURE
        )
        addItem(
            i++,
            R.mipmap.texture_unit,
            "纹理单元",
            "通过纹理单元让多个纹理显示在一个图形上",
            IMyNativeRendererType.SAMPLE_TYPE_TEXTURE2
        )
        addItem(
            i++,
            R.mipmap.transform,
            "变换",
            "使用变换矩阵实现物体的缩放、位移和旋转",
            IMyNativeRendererType.SAMPLE_TYPE_MAT
        )
        addItem(
            i++,
            R.mipmap.coordinate,
            "坐标系统",
            "使用矩阵构建坐标系，实现3D效果",
            IMyNativeRendererType.SAMPLE_TYPE_COORDINATE
        )
        addItem(
            i++,
            R.mipmap.coordinate,
            "摄像机",
            "使用摄像机矩阵，实现3D空间观察视角",
            IMyNativeRendererType.SAMPLE_TYPE_CAMERA
        )
        addItem(
            i++,
            R.mipmap.coordinate,
            "移动摄像机位置",
            "使用事件改变摄像机位置，包括按钮点击和手指滑动",
            IMyNativeRendererType.SAMPLE_TYPE_CAMERA_AUTO_MOVE
        )
        addItem(
            i++,
            R.mipmap.color_light,
            "创建一个光照场景",
            "用3D立方体创建一个基本的光源",
            IMyNativeRendererType.SAMPLE_TYPE_COLORS
        )
        addItem(
            i++,
            R.mipmap.color_light_view,
            "冯氏光照效果",
            "在视图空间中计算冯氏光照效果",
            IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW
        )
        addItem(
            i++,
            R.mipmap.material,
            "材质",
            "通过材质和光源颜色细微控制物体表面颜色",
            IMyNativeRendererType.SAMPLE_TYPE_COLORS_MATERIAL
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