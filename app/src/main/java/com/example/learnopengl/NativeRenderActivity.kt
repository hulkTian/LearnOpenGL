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
import androidx.annotation.RequiresApi
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

    var renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY

    @RequiresApi(Build.VERSION_CODES.TIRAMISU)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (!detectOpenGLES30()) {
            return
        }
        val intent = intent
        val uiConfig =
            intent.getSerializableExtra(IMyNativeRendererType.RENDER_TYPE, UIConfig::class.java)
        uiConfig?.let {
            type = it.type
            renderMode = it.renderMode
        }

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
                uiConfig?.let { config ->
                    initClickLayout(config)
                }

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
                glSurfaceView.renderMode = renderMode

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

    private fun initClickLayout(uiConfig: UIConfig) {
        binding.llClick.visibility = uiConfig.showClickLayout
        if (uiConfig.showClickLayout == View.VISIBLE) {
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
        }
        binding.btB.visibility = uiConfig.showBooleanButton
        if (uiConfig.showBooleanButton == View.VISIBLE) {
            binding.btB.setOnClickListener {
                mRenderer?.processInput(Key.KEY_B)
                mGLSurfaceView?.requestRender()
            }
        }
        binding.sbSeekBar.visibility = uiConfig.showSeekBar1
        binding.tvTitle.visibility = uiConfig.showSeekBar1
        if (uiConfig.showSeekBar1 == View.VISIBLE) {
            binding.tvTitle.text = uiConfig.title1
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
        }
        binding.sbSeekBar2.visibility = uiConfig.showSeekBar2
        binding.tvTitle2.visibility = uiConfig.showSeekBar2
        if (uiConfig.showSeekBar2 == View.VISIBLE) {
            binding.tvTitle2.text = uiConfig.title2
            binding.sbSeekBar2.setOnSeekBarChangeListener(object : OnSeekBarChangeListener {
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    mRenderer?.progressChanged2(progress)
                    mGLSurfaceView?.requestRender()
                }

                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            })
        }
        binding.sbSeekBar3.visibility = uiConfig.showSeekBar3
        binding.tvTitle3.visibility = uiConfig.showSeekBar3
        if (uiConfig.showSeekBar3 == View.VISIBLE) {
            binding.tvTitle3.text = uiConfig.title3
            binding.sbSeekBar3.setOnSeekBarChangeListener(object : OnSeekBarChangeListener {
                override fun onProgressChanged(
                    seekBar: SeekBar?,
                    progress: Int,
                    fromUser: Boolean
                ) {
                    mRenderer?.progressChanged3(progress)
                    mGLSurfaceView?.requestRender()
                }

                override fun onStartTrackingTouch(seekBar: SeekBar?) {
                }

                override fun onStopTrackingTouch(seekBar: SeekBar?) {
                }
            })
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