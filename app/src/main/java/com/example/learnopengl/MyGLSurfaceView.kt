package com.example.learnopengl

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import kotlin.math.cos
import kotlin.math.sin

class MyGLSurfaceView : GLSurfaceView {
    private lateinit var mRenderer: MyNativeRenderer

    private var mRatioWidth = 0
    private var mRatioHeight = 0

    //滑动灵敏度
    private var sensitivity = 0.05f

    // 上次触摸事件的坐标
    private var lastX = 0.0f
    private var lastY = 0.0f

    //欧拉角：pitch和yaw
    private var pitch = 0.0
    private var yaw = 0.0

    private var isFirst = true;

    constructor(context: Context?) : super(context) {}

    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs) {}

    constructor(context: Context?, glRender: MyNativeRenderer, eglContextVersion: Int) : this(
        context, null, glRender, eglContextVersion
    )

    constructor(
        context: Context?,
        attrs: AttributeSet?,
        glRender: MyNativeRenderer,
        eglContextVersion: Int
    ) : super(context, attrs) {
        setEGLContextClientVersion(eglContextVersion)
        mRenderer = glRender

        /*If no setEGLConfigChooser method is called,
        then by default the view will choose an RGB_888 surface with a depth buffer depth of at least 16 bits.*/
        // 最后 2 个参数表示分别配置 16 位的深度缓冲区和模板缓冲区
        setEGLConfigChooser(8, 8, 8, 8, 16, 8)
        setRenderer(mRenderer)
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val width = MeasureSpec.getSize(widthMeasureSpec)
        val height = MeasureSpec.getSize(heightMeasureSpec)
        if (0 == mRatioWidth || 0 == mRatioHeight) {
            setMeasuredDimension(width, height)
        } else {
            if (width < height * mRatioWidth / mRatioHeight) {
                setMeasuredDimension(width, width * mRatioHeight / mRatioWidth)
            } else {
                setMeasuredDimension(height * mRatioWidth / mRatioHeight, height)
            }
        }
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        if (event?.action == MotionEvent.ACTION_MOVE) {
            moveCallback(event.rawX, event.rawY)
        } else if (event?.action == MotionEvent.ACTION_DOWN) {
            lastX = event.rawX
            lastY = event.rawY
        }
        return true
    }

    private fun moveCallback(rawX: Float, rawY: Float) {
        //计算偏移量
        val offsetX = (rawX - lastX) * sensitivity
        val offsetY = (rawY - lastY) * sensitivity
        lastX = rawX
        lastY = rawY

        //保存偏移量
        yaw += offsetX
        pitch += offsetY

        //设置 pitch 视角范围
        if (pitch > 89.0)
            pitch = 89.0
        if (pitch < -89.0)
            pitch = -89.0

        // 方向向量的x分量
        val x = cos(Math.toRadians(yaw)) * cos(Math.toRadians(pitch))
        // 方向向量的y分量
        val y = sin(Math.toRadians(pitch))
        // 方向向量的z分量
        val z = sin(Math.toRadians(yaw)) * cos(Math.toRadians(pitch))
        if (isFirst) {
            mRenderer.moveCallback(0.0,0.0,-1.0)
            isFirst = false
        } else {
            mRenderer.moveCallback(x,y,z)
        }
    }

    fun setAspectRatio(width: Int, height: Int) {
        Log.d(TAG, "setAspectRatio() called with: width = [$width], height = [$height]")
        require(!(width < 0 || height < 0)) { "Size cannot be negative." }
        mRatioWidth = width
        mRatioHeight = height
        requestLayout()
    }

    companion object {
        private const val TAG = "MyGLSurfaceView"
    }
}