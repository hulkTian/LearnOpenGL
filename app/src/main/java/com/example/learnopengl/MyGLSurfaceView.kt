package com.example.learnopengl

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent

class MyGLSurfaceView : GLSurfaceView {
    private lateinit var mRenderer: MyNativeRenderer

    private var mRatioWidth = 0
    private var mRatioHeight = 0

    private var mLastTouchTime = 0
    private var mPreviousX = 0.0.toFloat()
    private var mPreviousY = 0.0.toFloat()

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
        // 最后 2 个参数表示分别配置 24 位的深度缓冲区和模板缓冲区
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
        val currentTimeMillis = System.currentTimeMillis()
        if (currentTimeMillis - mLastTouchTime > 200) {
            val y: Float = event!!.y
            val x: Float = event.x
            if (event.action == MotionEvent.ACTION_MOVE) {
                val dy: Float = y - mPreviousY
                val dx: Float = x - mPreviousX
                moveCallback(dx, dy)
                requestRender()
            }
            mPreviousY = y
            mPreviousX = x
        }
        return true
    }

    private fun moveCallback(x: Float, y: Float) {
        mRenderer.moveCallback(x, y)
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