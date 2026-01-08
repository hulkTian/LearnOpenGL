package com.example.learnopengl

import android.opengl.GLSurfaceView
import android.view.View
import java.io.Serializable

class BuildUI {
    var showClickLayout: Int = View.VISIBLE
    var showBooleanButton: Int = View.GONE

    var showRadioGroup: Int = View.GONE

    var showSeekBar1: Int = View.GONE
    var showSeekBar2: Int = View.GONE
    var showSeekBar3: Int = View.GONE
    var showSeekBar4: Int = View.GONE

    var type: Float = 0F
    var renderMode: Int = GLSurfaceView.RENDERMODE_WHEN_DIRTY

    var isEnable: Boolean = true

    fun setShowClickLayout(show: Int) = apply { showClickLayout = show }
    fun setShowBooleanButton(show: Int) = apply { showBooleanButton = show }

    fun setShowRadioGroup(show: Int) = apply { showRadioGroup = show }

    fun setShowSeekBar1(show: Int) = apply { showSeekBar1 = show }
    fun setShowSeekBar2(show: Int) = apply { showSeekBar2 = show }
    fun setShowSeekBar3(show: Int) = apply { showSeekBar3 = show }
    fun setShowSeekBar4(show: Int) = apply { showSeekBar4 = show }

    fun setEnable(enable: Boolean) = apply { isEnable = enable }

    var title1: String = "title1"
    var title2: String = "title2"
    var title3: String = "title3"
    var title4: String = "title4"

    var rb1: String = "rb1"
    var rb2: String = "rb2"
    var rb3: String = "rb3"
    var rb4: String = "rb4"
    var rb5: String = "rb5"

    fun setTitle1(text: String) = apply { title1 = text }
    fun setTitle2(text: String) = apply { title2 = text }
    fun setTitle3(text: String) = apply { title3 = text }
    fun setTitle4(text: String) = apply { title4 = text }

    fun setRadioButton1(text: String) = apply { rb1 = text }
    fun setRadioButton2(text: String) = apply { rb2 = text }
    fun setRadioButton3(text: String) = apply { rb3 = text }
    fun setRadioButton4(text: String) = apply { rb4 = text }
    fun setRadioButton5(text: String) = apply { rb5 = text }

    fun setType(t: Float) = apply { type = t }
    fun setRenderMode(t: Int) = apply { renderMode = t }

    fun build(): UIConfig {
        return UIConfig(
            showClickLayout,
            showBooleanButton,
            isEnable,
            showSeekBar1,
            showSeekBar2,
            showSeekBar3,
            showSeekBar4,
            title1,
            title2,
            title3,
            title4,
            showRadioGroup,
            rb1,
            rb2,
            rb3,
            rb4,
            rb5,
            type,
            renderMode
        )
    }
}

data class UIConfig(
    val showClickLayout: Int,
    val showBooleanButton: Int,
    val isEnable: Boolean,
    val showSeekBar1: Int,
    val showSeekBar2: Int,
    val showSeekBar3: Int,
    val showSeekBar4: Int,
    val title1: String,
    val title2: String,
    val title3: String,
    val title4: String,
    val showRadioGroup: Int,
    val rb1: String,
    val rb2: String,
    val rb3: String,
    val rb4: String,
    val rb5: String,
    val type: Float,
    val renderMode: Int
) : Serializable
