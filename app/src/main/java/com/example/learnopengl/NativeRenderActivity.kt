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
import android.view.View
import android.view.ViewGroup
import android.widget.RelativeLayout
import android.widget.SeekBar
import android.widget.SeekBar.OnSeekBarChangeListener
import android.widget.Toast
import androidx.core.app.ActivityCompat
import com.example.learnopengl.databinding.ActivityNativeRenderBinding

class NativeRenderActivity : Activity() {
    companion object {
        private const val CONTEXT_CLIENT_VERSION = 3
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

                initClickLayout()

                val lp = RelativeLayout.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
                )
                lp.addRule(RelativeLayout.CENTER_IN_PARENT)
                glSurfaceView.layoutParams = lp
                binding.rootView.addView(glSurfaceView, 0)

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

    private fun initClickLayout() {
        if (type == IMyNativeRendererType.SAMPLE_TYPE_CAMERA_AUTO_MOVE
            || type == IMyNativeRendererType.SAMPLE_TYPE_COLORS
            || type == IMyNativeRendererType.SAMPLE_TYPE_COLORS_VIEW
            || type == IMyNativeRendererType.SAMPLE_TYPE_COLORS_MATERIAL
            || type == IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_MAPS_DIFFUSE
            || type == IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_CASTERS_DIRECTIONAL
            || type == IMyNativeRendererType.SAMPLE_TYPE_LIGHTING_MERGE
            || type == IMyNativeRendererType.SAMPLE_TYPE_MODEL_LOADING
            || type == IMyNativeRendererType.SAMPLE_TYPE_DEPTH_TESTING
            || type == IMyNativeRendererType.SAMPLE_TYPE_STENCIL_TESTING
            || type == IMyNativeRendererType.SAMPLE_TYPE_BLENDING_DISCARD
            || type == IMyNativeRendererType.SAMPLE_TYPE_CULL_FACE
            || type == IMyNativeRendererType.SAMPLE_TYPE_FRAME_BUFFERS
            || type == IMyNativeRendererType.SAMPLE_TYPE_FRAME_BUFFERS_EXERCISE
            || type == IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS
            || type == IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFLECTION1
            || type == IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFLECTION2
            || type == IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_REFRACTION
            || type == IMyNativeRendererType.SAMPLE_TYPE_CUBE_MAPS_EXERCISE
            || type == IMyNativeRendererType.SAMPLE_TYPE_INSTANCING_ASTEROIDS
            || type == IMyNativeRendererType.SAMPLE_TYPE_ANIT_ALIASING
            || type == IMyNativeRendererType.SAMPLE_TYPE_ADVANCED_LIGHTING
            || type == IMyNativeRendererType.SAMPLE_TYPE_ADVANCED_LIGHTING_GAMMA_CORRECTED
            || type == IMyNativeRendererType.SAMPLE_TYPE_SHADOW_MAPPING_BASE
            || type == IMyNativeRendererType.SAMPLE_TYPE_NORMAL_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_PARALLAX_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_STEEP_PARALLAX_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_PARALLAX_OCCLUSION_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_HDR
            || type == IMyNativeRendererType.SAMPLE_TYPE_BLOOM
            || type == IMyNativeRendererType.SAMPLE_TYPE_DEFERRED_SHADING
            || type == IMyNativeRendererType.SAMPLE_TYPE_SSAO
            || type == IMyNativeRendererType.SAMPLE_TYPE_PBR_LIGHTING
            || type == IMyNativeRendererType.SAMPLE_TYPE_PBR_LIGHTING_TEXTURED
            || type == IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_IRRADIANCE_CONVERSION
            || type == IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_IRRADIANCE
            || type == IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_SPECULAR
            || type == IMyNativeRendererType.SAMPLE_TYPE_PBR_IBL_SPECULAR_TEXTURED
            || type == IMyNativeRendererType.SAMPLE_TYPE_BREAK_OUT
            || type == IMyNativeRendererType.SAMPLE_TYPE_WEIGHTED_BLENDED_OIT
            || type == IMyNativeRendererType.SAMPLE_TYPE_DEPTH_DISPLAY
        ) {
            binding.llClick.visibility = View.VISIBLE
            binding.btW.setOnClickListener {
                mRenderer?.processInput(Key.KEY_W)
                mGLSurfaceView?.requestRender()
            }
            binding.btS.setOnClickListener {
                mRenderer?.processInput(Key.KEY_S)
                mGLSurfaceView?.requestRender()
            }
            binding.btA.setOnClickListener {
                mRenderer?.processInput(Key.KEY_A)
                mGLSurfaceView?.requestRender()
            }
            binding.btD.setOnClickListener {
                mRenderer?.processInput(Key.KEY_D)
                mGLSurfaceView?.requestRender()
            }
            if (type == IMyNativeRendererType.SAMPLE_TYPE_ADVANCED_LIGHTING_GAMMA_CORRECTED
                || type == IMyNativeRendererType.SAMPLE_TYPE_HDR
                || type == IMyNativeRendererType.SAMPLE_TYPE_BLOOM
            ) {
                binding.btB.visibility = View.VISIBLE
                binding.btB.setOnClickListener {
                    mRenderer?.processInput(Key.KEY_B)
                    mGLSurfaceView?.requestRender()
                }
            } else {
                binding.btB.visibility = View.GONE
            }
        } else {
            binding.llClick.visibility = View.GONE
        }

        if (type == IMyNativeRendererType.SAMPLE_TYPE_HDR || type == IMyNativeRendererType.SAMPLE_TYPE_BLOOM) {
            binding.sbSeekBar.setOnSeekBarChangeListener(object : OnSeekBarChangeListener {
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    mRenderer?.progressChanged(progress)
                    mGLSurfaceView?.requestRender()
                }

                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            })
        } else {
            binding.sbSeekBar.visibility = View.GONE
        }
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
        if (type == IMyNativeRendererType.SAMPLE_TYPE_UNIFORM
            || type == IMyNativeRendererType.SAMPLE_TYPE_MAT
            //|| type == IMyNativeRendererType.SAMPLE_TYPE_COORDINATE
            || type == IMyNativeRendererType.SAMPLE_TYPE_CAMERA
            || type == IMyNativeRendererType.SAMPLE_TYPE_COLORS_MATERIAL
            || type == IMyNativeRendererType.SAMPLE_TYPE_NORMAL_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_PARALLAX_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_STEEP_PARALLAX_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_PARALLAX_OCCLUSION_MAPPING
            || type == IMyNativeRendererType.SAMPLE_TYPE_BREAK_OUT
        ) {
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