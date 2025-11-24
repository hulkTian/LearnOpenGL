package com.example.learnopengl

import android.opengl.GLSurfaceView
import android.view.View
import java.io.Serializable

class BuildUI {
    var showClickLayout: Int = View.VISIBLE
    var showBooleanButton: Int = View.GONE

    var showSeekBar1: Int = View.GONE
    var showSeekBar2: Int = View.GONE
    var showSeekBar3: Int = View.GONE

    var type: Int = 0
    var renderMode: Int = GLSurfaceView.RENDERMODE_WHEN_DIRTY

    fun setShowClickLayout(show: Int) = apply { showClickLayout = show }
    fun setShowBooleanButton(show: Int) = apply { showBooleanButton = show }

    fun setShowSeekBar1(show: Int) = apply { showSeekBar1 = show }
    fun setShowSeekBar2(show: Int) = apply { showSeekBar2 = show }
    fun setShowSeekBar3(show: Int) = apply { showSeekBar3 = show }

    var title1: String = "title1"
    var title2: String = "title2"
    var title3: String = "title3"

    fun setTitle1(text: String) = apply { title1 = text }
    fun setTitle2(text: String) = apply { title2 = text }
    fun setTitle3(text: String) = apply { title3 = text }

    fun setType(t: Int) = apply { type = t }
    fun setRenderMode(t: Int) = apply { renderMode = t }

    fun build(): UIConfig {
        return UIConfig(
            showClickLayout,
            showBooleanButton,
            showSeekBar1,
            showSeekBar2,
            showSeekBar3,
            title1,
            title2,
            title3,
            type,
            renderMode
        )
    }
}

data class UIConfig(
    val showClickLayout: Int,
    val showBooleanButton: Int,
    val showSeekBar1: Int,
    val showSeekBar2: Int,
    val showSeekBar3: Int,
    val title1: String,
    val title2: String,
    val title3: String,
    val type: Int,
    val renderMode: Int
) : Serializable
