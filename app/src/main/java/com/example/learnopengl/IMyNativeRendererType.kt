package com.example.learnopengl

object IMyNativeRendererType {
    const val RENDER_TYPE: String = "RENDER_TYPE"
    const val SAMPLE_TYPE = 100

    const val SAMPLE_TYPE_TRIANGLE = SAMPLE_TYPE
    const val SAMPLE_TYPE_RECTANGLE = SAMPLE_TYPE + 1
    const val SAMPLE_TYPE_TRIANGLE2 = SAMPLE_TYPE + 2
    const val SAMPLE_TYPE_TRIANGLE3 = SAMPLE_TYPE + 3
    const val SAMPLE_TYPE_TRIANGLE4 = SAMPLE_TYPE + 4
    const val SAMPLE_TYPE_TRIANGLE5 = SAMPLE_TYPE + 5
    const val SAMPLE_TYPE_TEXTURE = SAMPLE_TYPE + 6
    const val SAMPLE_TYPE_TEXTURE2 = SAMPLE_TYPE + 7
    const val SAMPLE_TYPE_MAT = SAMPLE_TYPE + 8
    const val SAMPLE_TYPE_COORDINATE = SAMPLE_TYPE + 9
    const val SAMPLE_TYPE_CAMERA = SAMPLE_TYPE + 10
    const val SAMPLE_TYPE_CAMERA_AUTO_MOVE = SAMPLE_TYPE + 11
    const val SAMPLE_TYPE_COLORS = SAMPLE_TYPE + 12
    const val SAMPLE_TYPE_COLORS_VIEW = SAMPLE_TYPE + 13
    const val SAMPLE_TYPE_COLORS_MATERIAL = SAMPLE_TYPE + 14
    const val SAMPLE_TYPE_LIGHTING_MAPS_DIFFUSE = SAMPLE_TYPE + 15
    const val SAMPLE_TYPE_LIGHTING_CASTERS_DIRECTIONAL = SAMPLE_TYPE + 16
}

object Key {
    const val KEY_W = 1
    const val KEY_S = 2
    const val KEY_A = 3
    const val KEY_D = 4
}