package com.example.learnopengl

import android.Manifest
import android.app.Activity
import android.app.ActivityManager
import android.content.Context
import android.content.pm.PackageManager
import android.opengl.GLSurfaceView
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.ViewGroup
import android.widget.RelativeLayout
import android.widget.Toast
import androidx.core.app.ActivityCompat
import com.example.learnopengl.databinding.ActivityNativeRenderBinding

class NativeRenderActivity : Activity() {
    companion object {
        private const val MIN_DIALOG = 1
        private const val CONTEXT_CLIENT_VERSION = 3
        private const val MAG_DIALOG = 2
        private const val MIN_SETTING = "min_setting"
        private const val MAG_SETTING = "mag_setting"
        private const val TAG: String = "NativeRenderActivity"

        private val REQUEST_PERMISSIONS = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO
        )
        private const val PERMISSION_REQUEST_CODE = 1
    }

    private lateinit var binding: ActivityNativeRenderBinding

    //渲染器，通过JNI交给native层处理
    private var mRenderer: MyNativeRenderer? = null

    //自定义的GLSurfaceView，通过JNI交给native层处理
    private var mGLSurfaceView: MyGLSurfaceView? = null

    //当前需要渲染的内容类型
    var type = IMyNativeRendererType.SAMPLE_TYPE

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (!detectOpenGLES30()) {
            return
        }
        val intent = intent
        type = intent.getIntExtra(
            IMyNativeRendererType.RENDER_TYPE,
            IMyNativeRendererType.SAMPLE_TYPE
        )

        mRenderer = MyNativeRenderer(this)
        mRenderer?.let { myNativeRenderer ->
            //必须先设置渲染类型，否则无法确定需要展示的类容
            myNativeRenderer.setRendererType(type)

            //配置渲染控件
            mGLSurfaceView = MyGLSurfaceView(this, myNativeRenderer, CONTEXT_CLIENT_VERSION)

            mGLSurfaceView?.let { glSurfaceView ->
                binding = ActivityNativeRenderBinding.inflate(layoutInflater)
                setContentView(binding.root)

                binding.ivRootBack.setOnClickListener {
                    finish()
                }

                val lp = RelativeLayout.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
                )
                lp.addRule(RelativeLayout.CENTER_IN_PARENT)
                lp.topMargin = 140
                binding.rootView.addView(glSurfaceView, lp)

                if (binding.rootView.width != glSurfaceView.width
                    || binding.rootView.height != glSurfaceView.height
                ) {
                    glSurfaceView.setAspectRatio(binding.rootView.width, binding.rootView.height)
                }

                //设置渲染模式
                setRenderMode(glSurfaceView)

                //加载渲染内容
                loadRenderContent()

                //请求渲染
                glSurfaceView.requestRender()
            }
        }
    }

    private fun loadRenderContent() {
        Log.i("", "===================")
    }

    override fun onResume() {
        super.onResume()
        mGLSurfaceView?.onResume()
    }

    override fun onPause() {
        super.onPause()
        mGLSurfaceView?.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        mRenderer?.onDestroy()
    }

    /**
     * 检测设备是否支持OpenGL ES 3.0
     */
    private fun detectOpenGLES30(): Boolean {
        val am = getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val info = am.deviceConfigurationInfo
        val reqGlEsVersion = info.reqGlEsVersion

        // 有些GPU模拟部分有缺陷，为了使代码在模拟器上正常工作，写下面的代码
        // 这段代码判断当前设备是不是模拟器，如果是，就假定它支持OpenGL ES 3.0
        // 要确保程序能运行，模拟器一定要配置OpenGL ES 3.0
        val isSupportedOpenGLES30 = reqGlEsVersion >= 0x30000
                || (Build.FINGERPRINT.startsWith("generic")
                || Build.FINGERPRINT.startsWith("unknown")
                || Build.MODEL.contains("google_sdk")
                || Build.MODEL.contains("Emulator")
                || Build.MODEL.contains("Android SDK built for x86"))

        if (!isSupportedOpenGLES30) {
            Log.e(
                TAG,
                "OpenGL ES 3.0 not supported on device. The device's reqGlEsVersion is $reqGlEsVersion, Exiting..."
            )
            Toast.makeText(
                this,
                "当前设备不支持OpenGL ES 3.0 ，当前设备的GlEs版本是$reqGlEsVersion",
                Toast.LENGTH_LONG
            ).show()
        }
        return isSupportedOpenGLES30
    }

    /**
     * 根据渲染类型设置渲染模式.
     */
    private fun setRenderMode(glSurfaceView: MyGLSurfaceView) {
        if (type == IMyNativeRendererType.SAMPLE_TYPE_TRIANGLE5
            || type == IMyNativeRendererType.SAMPLE_TYPE_MAT
            || type == IMyNativeRendererType.SAMPLE_TYPE_COORDINATE) {
            glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        } else {
            glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
        }
    }

    private fun hasPermissionsGranted(permissions: Array<String>): Boolean {
        for (permission in permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                != PackageManager.PERMISSION_GRANTED
            ) {
                return false
            }
        }
        return true
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String?>,
        grantResults: IntArray
    ) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                Toast.makeText(
                    this,
                    "We need the permission: RECORD_AUDIO && WRITE_EXTERNAL_STORAGE",
                    Toast.LENGTH_SHORT
                ).show()
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }
}