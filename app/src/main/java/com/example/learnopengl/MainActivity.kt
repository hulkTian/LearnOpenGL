package com.example.learnopengl

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.SparseIntArray
import android.widget.TextView
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
            "展示如何使用VAO和VBO绘制三角形",
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
            "给三角形增加颜色顶点属性，理解每个像素的颜色插值来源",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE2
        )

        addItem(
            i++,
            R.mipmap.triangle3,
            "绘制两个三角形",
            "添加更多顶点到数据中，使用glDrawArrays绘制两个相连的三角形",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE3
        )

        addItem(
            i++,
            R.mipmap.triangle3,
            "绘制两个三角形",
            "使用不同的VAO和VBO绘制两个三角形",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE4
        )

        addItem(
            i++,
            R.mipmap.triangle,
            "着色器语言GLSL",
            "使用Uniform在片段着色器声明一个变量，实现一个颜色会变化的三角形",
            IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE5
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