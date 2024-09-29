package com.example.learnopengl

import android.app.Activity
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import android.util.Log
import android.view.MotionEvent
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyNativeRenderer(activity: Activity) : GLSurfaceView.Renderer {
    private var mActivity: Activity = activity

    init {
        System.loadLibrary("learn-opengles-lib")
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        val assetManager: AssetManager = mActivity.assets
        nativeOnSurfaceCreated(assetManager, mActivity.filesDir.absolutePath)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        nativeOnDrawFrame()
    }

    fun setRendererType(renderSampleType: Int) {
        nativeSetRenderType(renderSampleType)
    }

    fun processInput(key: Int) {
        nativeProcessInput(key)
    }

    fun moveCallback(x: Float, y: Float) {
        nativeMoveCallback(x, y)
    }

    fun onDestroy() {
        nativeOnDestroy()
    }

    private external fun nativeOnSurfaceCreated(assetManager: AssetManager, pathToInternalDir:String)
    private external fun nativeOnSurfaceChanged(width: Int, height: Int)
    private external fun nativeOnDrawFrame()
    private external fun nativeSetRenderType(renderSampleType: Int)
    private external fun nativeOnDestroy()

    /**
     * 处理按钮点击事件：
     */
    private external fun nativeProcessInput(key: Int)

    private external fun nativeMoveCallback(x: Float, y: Float)

}