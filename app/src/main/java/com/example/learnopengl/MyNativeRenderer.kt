package com.example.learnopengl

import android.app.Activity
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyNativeRenderer(activity: Activity) : GLSurfaceView.Renderer {
    private var mActivity: Activity = activity

    init {
        System.loadLibrary("learn-opengles-lib")
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        val assetManager: AssetManager = mActivity.assets
        nativeOnSurfaceCreated(assetManager)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        nativeOnDrawFrame()
    }

    fun setRendererType(renderSampleType: Float) {
        nativeSetRenderType(renderSampleType)
    }

    fun processInput(key: Int) {
        nativeProcessInput(key)
    }

    fun progressChanged(progress: Int) {
        nativeProgressChanged(progress)
    }

    fun progressChanged2(progress: Int) {
        nativeProgressChanged2(progress)
    }

    fun progressChanged3(progress: Int) {
        nativeProgressChanged3(progress)
    }

    fun progressChanged4(progress: Int) {
        nativeProgressChanged4(progress)
    }

    fun moveCallback(x: Float, y: Float) {
        nativeMoveCallback(x, y)
    }

    fun onDestroy() {
        nativeOnDestroy()
    }

    private external fun nativeOnSurfaceCreated(assetManager: AssetManager)
    private external fun nativeOnSurfaceChanged(width: Int, height: Int)
    private external fun nativeOnDrawFrame()
    private external fun nativeSetRenderType(renderSampleType: Float)
    private external fun nativeOnDestroy()

    /**
     * 处理按钮点击事件：
     */
    private external fun nativeProcessInput(key: Int)
    private external fun nativeProgressChanged(progress: Int)
    private external fun nativeProgressChanged2(progress: Int)
    private external fun nativeProgressChanged3(progress: Int)
    private external fun nativeProgressChanged4(progress: Int)

    private external fun nativeMoveCallback(x: Float, y: Float)

}