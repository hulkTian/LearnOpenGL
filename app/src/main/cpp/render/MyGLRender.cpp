//
// Created by TS on 2024/2/27.
//

#include "MyGLRender.h"
/*#include "1_getting_started/1_vao_vbo/vao_vbo.h"
#include "1_getting_started/2_ebo/ebo.h"
#include "1_getting_started/6_transform/Transform.h"
#include "1_getting_started/7_coordinate/CoordinateSystems.h"
#include "1_getting_started/8_camera/Camera.h"
#include "1_getting_started/8_camera/CameraAutoMove.h"
#include "2_light/1_colors/ColorsLight.h"
#include "2_light/2_base_lighting/ColorsAtView.h"
#include "2_light/3_material/Material.h"
#include "2_light/4_light_map/lighting_maps_diffuse.h"
#include "3_mesh/ModelLoading.h"
#include "2_light/6_multiple_lights/multiple_lights.h"
#include "4_advanced_opengl/1_depth_testing/1_depth_testing/DepthTesting.h"
#include "4_advanced_opengl/2_stencil_testing/StencilTesting.h"
#include "4_advanced_opengl/3_blending_discard/BlendingDiscard.h"
#include "4_advanced_opengl/4_cull_face/CullFace.h"
#include "4_advanced_opengl/5_frame_buffer/FrameBuffers.h"
#include "4_advanced_opengl/5_frame_buffer/FrameBuffersExercise.h"
#include "4_advanced_opengl/6_cube_maps/CubeMaps.h"
#include "4_advanced_opengl/6_cube_maps/CubeMapsReflection.h"
#include "4_advanced_opengl/6_cube_maps/ReflectionMode.h"
#include "4_advanced_opengl/6_cube_maps/CubeMapsRefraction.h"
#include "4_advanced_opengl/6_cube_maps/CubeMapsReflectionExercise.h"
#include "4_advanced_opengl/7_ubo/AdvancedUbo.h"
#include "4_advanced_opengl/9_geometry_shader/GeometryShaderPoint.h"
#include "4_advanced_opengl/10_instancing/Instancing.h"
#include "4_advanced_opengl/10_instancing/Asteroids.h"
#include "4_advanced_opengl/11_anti_aliasing/AnitAliasing.h"
#include "5_advanced_lighting/1_blinn_phong/AdvancedLighting.h"
#include "5_advanced_lighting/2_gamma_correction/gamma_correction.h"
#include "5_advanced_lighting/3_shadow_mapping_depth/shadow_mapping_depth.h"
#include "5_advanced_lighting/4_shadow_mapping_base/shadow_mapping_base.h"
#include "5_advanced_lighting/5_normal_mapping/normal_mapping.h"
#include "5_advanced_lighting/6_parallax_mapping/parallax_mapping.h"
#include "5_advanced_lighting/7_steep_parallax_mapping/steep_parallax_mapping.h"
#include "5_advanced_lighting/8_parallax_occlusion_mapping/parallax_occlusion_mapping.h"
#include "5_advanced_lighting/9_hdr/hdr.h"
#include "5_advanced_lighting/10_bloom/bloom.h"
#include "5_advanced_lighting/11_deferred_shading/deferred_shading.h"
#include "5_advanced_lighting/12_ssao/ssao.h"
#include "6_pbr/1_lighting/pbr_lighting.h"
#include "6_pbr/2_lighting_textured/lighting_textured.h"
#include "6_pbr/3_ibl_irradiance_conversion/ibl_irradiance_conversion.h"
#include "6_pbr/4_ibl_irradiance/ibl_irradiance.h"
#include "6_pbr/5_ibl_specular/ibl_specular.h"
#include "6_pbr/6_ibl_specular_textured/ibl_specular_textured.h"
#include "7_text_rendering/text_rendering.h"
#include "8_debugging/debugging.h"
#include "9_break_out/Breakout.h"
#include "10_guest/1_oit/weighted_blended.h"
#include "1_getting_started/5_texture/texture_unit.h"
#include "1_getting_started/5_texture/texture_warp.h"
#include "1_getting_started/3_triangle_exercise/1_triangle_exercise.h"
#include "1_getting_started/3_triangle_exercise/2_triangle_exercise.h"
#include "1_getting_started/4_uniform/uniform.h"
#include "4_advanced_opengl/1_depth_testing/2_depth_visualization/depth_display.h"
#include "1_getting_started/5_texture/texture_exercise_1.h"
#include "1_getting_started/5_texture/texture_exercise_2.h"
#include "1_getting_started/5_texture/texture_exercise_3.h"
#include "1_getting_started/5_texture/texture_exercise_4.h"
#include "1_getting_started/7_coordinate/CoordinateSystemsExercise_1.h"
#include "1_getting_started/7_coordinate/CoordinateSystemsExercise_2.h"
#include "1_getting_started/7_coordinate/CoordinateSystemsExercise_3.h"
#include "2_light/1_colors/ColorsLightExercise1.h"
#include "2_light/1_colors/ColorsLightExercise2.h"
#include "2_light/1_colors/ColorsLightExercise3.h"
#include "2_light/1_colors/ColorsLightExercise4.h"
#include "2_light/3_material/MaterialExercise.h"
#include "2_light/4_light_map/lighting_maps_specular.h"
#include "2_light/4_light_map/lighting_maps_exercise_1.h"
#include "2_light/4_light_map/lighting_maps_exercise_2.h"
#include "2_light/4_light_map/lighting_maps_exercise_3.h"
#include "2_light/4_light_map/lighting_maps_exercise_4.h"
#include "2_light/5_light_caster/light_casters_directional.h"
#include "2_light/5_light_caster/light_casters_point.h"*/
#include "SampleFactory.h"
#include <exception>

struct MyGLException : public std::exception {
    std::runtime_error m;

public:
    explicit MyGLException(const std::string &msg) : m(msg) {}

    const char *what() const noexcept override {
            return m.what();
    }
};

MyGLRender *MyGLRender::m_instance = nullptr;

MyGLRender::MyGLRender() {
    LOGD("MyGLRender::MyGLRender()")
    m_before_sample = nullptr;
    m_curr_sample = nullptr;
}

MyGLRender::~MyGLRender() {
    LOGD("MyGLRender::~MyGLRender")

    if (m_curr_sample) {
        m_curr_sample->Shutdown();
        delete m_curr_sample;
        m_curr_sample = nullptr;
    }

    if (m_before_sample) {
        m_before_sample->Shutdown();
        delete m_before_sample;
        m_before_sample = nullptr;
    }
}

void MyGLRender::SetRenderType(float renderSampleType) {
    m_before_sample = m_curr_sample;
    m_curr_sample = SampleFactory::Instance().Create(renderSampleType);
    if (!m_curr_sample) {
        throw MyGLException("未注册该Sample类型");
    }
    LOGD("MyGLRender::SetRenderType m_before_sample = %p, m_curr_sample=%p",
         m_before_sample, m_curr_sample)
}

/*void MyGLRender::SetRenderType(float renderSampleType) {
    LOGD("MyGLRender::SetRenderType renderSampleType = %f", renderSampleType)
    LOGD("MyGLRenderContext::SetRenderType 0 m_pBeforeSample = %p", m_before_sample)
    m_before_sample = m_curr_sample;

    if (renderSampleType == SAMPLE_TYPE_VAO_VBO) {
        m_curr_sample = new vao_vbo();
    } else if (renderSampleType == SAMPLE_TYPE_EBO) {
        m_curr_sample = new ebo();
    } else if (renderSampleType == SAMPLE_TYPE_VAO_VBO_EXERCISE_1) {
        m_curr_sample = new triangle_exercise_1();
    } else if (renderSampleType == SAMPLE_TYPE_VAO_VBO_EXERCISE_2) {
        m_curr_sample = new triangle_exercise_2();
    } else if (renderSampleType == SAMPLE_TYPE_UNIFORM) {
        m_curr_sample = new uniform();
    } else if (renderSampleType == SAMPLE_TYPE_TEXTURE_WARP) {
        m_curr_sample = new texture_warp();
    } else if (renderSampleType == SAMPLE_TYPE_TEXTURE_UNIT) {
        m_curr_sample = new texture_unit();
    } else if (renderSampleType == SAMPLE_TYPE_TEXTURE_EXERCISE_1) {
        m_curr_sample = new texture_exercise_1();
    } else if (renderSampleType == SAMPLE_TYPE_TEXTURE_EXERCISE_2) {
        m_curr_sample = new texture_exercise_2();
    } else if (renderSampleType == SAMPLE_TYPE_TEXTURE_EXERCISE_3) {
        m_curr_sample = new texture_exercise_3();
    } else if (renderSampleType == SAMPLE_TYPE_TEXTURE_EXERCISE_4) {
        m_curr_sample = new texture_exercise_4();
    } else if (renderSampleType == SAMPLE_TYPE_MAT) {
        m_curr_sample = new Transform();
    } else if (renderSampleType == SAMPLE_TYPE_COORDINATE) {
        m_curr_sample = new CoordinateSystems();
    } else if (renderSampleType == SAMPLE_TYPE_COORDINATE_EXERCISE_1) {
        m_curr_sample = new CoordinateSystemsExercise_1();
    } else if (renderSampleType == SAMPLE_TYPE_COORDINATE_EXERCISE_2) {
        m_curr_sample = new CoordinateSystemsExercise_2();
    } else if (renderSampleType == SAMPLE_TYPE_COORDINATE_EXERCISE_3) {
        m_curr_sample = new CoordinateSystemsExercise_3();
    } else if (renderSampleType == SAMPLE_TYPE_CAMERA) {
        m_curr_sample = new Camera();
    } else if (renderSampleType == SAMPLE_TYPE_CAMERA_AUTO_MOVE) {
        m_curr_sample = new CameraAutoMove();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS) {
        m_curr_sample = new ColorsLight();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS_VIEW) {
        m_curr_sample = new ColorsAtView();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS_VIEW_EXERCISE_1) {
        m_curr_sample = new ColorsLightExercise1();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS_VIEW_EXERCISE_2) {
        m_curr_sample = new ColorsLightExercise2();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS_VIEW_EXERCISE_3) {
        m_curr_sample = new ColorsLightExercise3();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS_VIEW_EXERCISE_4) {
        m_curr_sample = new ColorsLightExercise4();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS_MATERIAL) {
        m_curr_sample = new Material();
    } else if (renderSampleType == SAMPLE_TYPE_COLORS_MATERIAL_EXERCISE) {
        m_curr_sample = new MaterialExercise();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_MAPS_DIFFUSE) {
        m_curr_sample = new LightingMapsDiffuse();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_MAPS_SPECULAR) {
        m_curr_sample = new lighting_maps_specular();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_MAPS_EXERCISE_1) {
        m_curr_sample = new lighting_maps_exercise_1();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_MAPS_EXERCISE_2) {
        m_curr_sample = new lighting_maps_exercise_2();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_MAPS_EXERCISE_3) {
        m_curr_sample = new lighting_maps_exercise_3();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_MAPS_EXERCISE_4) {
        m_curr_sample = new lighting_maps_exercise_4();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_CASTERS_DIRECTIONAL) {
        m_curr_sample = new light_casters_directional();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_CASTERS_POINT) {
        m_curr_sample = new light_casters_point();
    } else if (renderSampleType == SAMPLE_TYPE_LIGHTING_MERGE) {
        m_curr_sample = new MultipleLights();
    } else if (renderSampleType == SAMPLE_TYPE_MODEL_LOADING) {
        m_curr_sample = new ModelLoading();
    } else if (renderSampleType == SAMPLE_TYPE_DEPTH_TESTING) {
        m_curr_sample = new DepthTesting();
    } else if (renderSampleType == SAMPLE_TYPE_DEPTH_DISPLAY) {
        m_curr_sample = new depth_display();
    } else if (renderSampleType == SAMPLE_TYPE_STENCIL_TESTING) {
        m_curr_sample = new StencilTesting();
    } else if (renderSampleType == SAMPLE_TYPE_BLENDING_DISCARD) {
        m_curr_sample = new BlendingDiscard();
    } else if (renderSampleType == SAMPLE_TYPE_CULL_FACE) {
        m_curr_sample = new CullFace();
    } else if (renderSampleType == SAMPLE_TYPE_FRAME_BUFFERS) {
        m_curr_sample = new FrameBuffers();
    } else if (renderSampleType == SAMPLE_TYPE_FRAME_BUFFERS_EXERCISE) {
        m_curr_sample = new FrameBuffersExercise();
    } else if (renderSampleType == SAMPLE_TYPE_CUBE_MAPS) {
        m_curr_sample = new CubeMaps();
    } else if (renderSampleType == SAMPLE_TYPE_CUBE_MAPS_REFLECTION1) {
        m_curr_sample = new CubeMapsReflection();
    } else if (renderSampleType == SAMPLE_TYPE_CUBE_MAPS_REFLECTION2) {
        m_curr_sample = new ReflectionMode();
    } else if (renderSampleType == SAMPLE_TYPE_CUBE_MAPS_REFLECTION) {
        m_curr_sample = new CubeMapsRefraction();
    } else if (renderSampleType == SAMPLE_TYPE_CUBE_MAPS_EXERCISE) {
        m_curr_sample = new CubeMapsReflectionExercise();
    } else if (renderSampleType == SAMPLE_TYPE_CUBE_UNIFORM_BUFFER) {
        m_curr_sample = new AdvancedUbo();
    } else if (renderSampleType == SAMPLE_TYPE_GEOMETRY_SHADER) {
        m_curr_sample = new GeometryShaderPoint();
    } else if (renderSampleType == SAMPLE_TYPE_INSTANCING) {
        m_curr_sample = new Instancing();
    } else if (renderSampleType == SAMPLE_TYPE_INSTANCING_ASTEROIDS) {
        m_curr_sample = new Asteroids();
    } else if (renderSampleType == SAMPLE_TYPE_ANIT_ALIASING) {
        m_curr_sample = new AnitAliasing();
    } else if (renderSampleType == SAMPLE_TYPE_ADVANCED_LIGHTING) {
        m_curr_sample = new AdvancedLighting();
    } else if (renderSampleType == SAMPLE_TYPE_ADVANCED_LIGHTING_GAMMA_CORRECTED) {
        m_curr_sample = new gamma_correction();
    } else if (renderSampleType == SAMPLE_TYPE_SHADOW_MAPPING_DEPTH) {
        m_curr_sample = new shadow_mapping_depth();
    } else if (renderSampleType == SAMPLE_TYPE_SHADOW_MAPPING_BASE) {
        m_curr_sample = new shadow_mapping_base();
    } else if (renderSampleType == SAMPLE_TYPE_NORMAL_MAPPING) {
        m_curr_sample = new normal_mapping();
    } else if (renderSampleType == SAMPLE_TYPE_PARALLAX_MAPPING) {
        m_curr_sample = new parallax_mapping();
    } else if (renderSampleType == SAMPLE_TYPE_STEEP_PARALLAX_MAPPING) {
        m_curr_sample = new steep_parallax_mapping();
    } else if (renderSampleType == SAMPLE_TYPE_PARALLAX_OCCLUSION_MAPPING) {
        m_curr_sample = new parallax_occlusion_mapping();
    } else if (renderSampleType == SAMPLE_TYPE_HDR) {
        m_curr_sample = new hdr();
    } else if (renderSampleType == SAMPLE_TYPE_BLOOM) {
        m_curr_sample = new bloom();
    } else if (renderSampleType == SAMPLE_TYPE_DEFERRED_SHADING) {
        m_curr_sample = new deferred_shading();
    } else if (renderSampleType == SAMPLE_TYPE_SSAO) {
        m_curr_sample = new ssao();
    } else if (renderSampleType == SAMPLE_TYPE_PBR_LIGHTING) {
        m_curr_sample = new pbr_lighting();
    } else if (renderSampleType == SAMPLE_TYPE_PBR_LIGHTING_TEXTURED) {
        m_curr_sample = new lighting_textured();
    } else if (renderSampleType == SAMPLE_TYPE_PBR_IBL_IRRADIANCE_CONVERSION) {
        m_curr_sample = new ibl_irradiance_conversion();
    } else if (renderSampleType == SAMPLE_TYPE_PBR_IBL_IRRADIANCE) {
        m_curr_sample = new ibl_irradiance();
    } else if (renderSampleType == SAMPLE_TYPE_PBR_IBL_SPECULAR) {
        m_curr_sample = new ibl_specular();
    } else if (renderSampleType == SAMPLE_TYPE_PBR_IBL_SPECULAR_TEXTURED) {
        m_curr_sample = new ibl_specular_textured();
    } else if (renderSampleType == SAMPLE_TYPE_TEXT_RENDERING) {
        m_curr_sample = new text_rendering();
    } else if (renderSampleType == SAMPLE_TYPE_DEBUGGING) {
        m_curr_sample = new debugging();
    } else if (renderSampleType == SAMPLE_TYPE_BREAK_OUT) {
        m_curr_sample = new Breakout();
    } else if (renderSampleType == SAMPLE_TYPE_WEIGHTED_BLENDED_OIT) {
        m_curr_sample = new weighted_blended();
    } else {
        // do nothing
    }

    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::SetRenderType() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    }
    LOGD("MyGLRender::SetRenderType m_before_sample = %p, m_curr_sample=%p",
         m_before_sample, m_curr_sample)
}*/

void MyGLRender::OnSurfaceCreated(JNIEnv *env, jobject assetManager) {
    LOGD("MyGLRender::OnSurfaceCreated")

    // 初始化设置assetManager  一定要记得初始化，否则会报空指针异常
    GLUtils::setEnvAndAssetManager(env, assetManager);

    if (m_before_sample) {
        m_before_sample->Shutdown();
        delete m_before_sample;
        m_before_sample = nullptr;
    }
    if (m_curr_sample) {
        LOGD("Create start")
        m_curr_sample->Create();
        LOGD("Create end")
    }
}

void MyGLRender::OnSurfaceChanged(int width, int height) {
    LOGD("MyGLRender::OnSurfaceChanged")
    if (m_curr_sample) {
        m_curr_sample->Change(width, height);
    }
}

void MyGLRender::OnDrawFrame() {
    if (m_curr_sample) {
        LOGD("Draw start")
        m_curr_sample->Draw();
        LOGD("Draw end")
    }
}

MyGLRender* MyGLRender::GetInstance() {
    if (m_instance == nullptr) {
        m_instance = new MyGLRender();
    }
    return m_instance;
}

void MyGLRender::DestroyInstance() {
    LOGD("MyGLRenderContext::DestroyInstance")
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

void MyGLRender::ProcessInput(int i) {
    LOGD("MyGLRender::ProcessInput key = %d", i)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::ProcessInput() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->ProcessInput(i);
    }
}

void MyGLRender:: ProgressChanged(int i) {
    LOGD("MyGLRender::ProgressChanged key = %d", i)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::ProgressChanged() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->ProgressChanged(i);
    }
}

void MyGLRender:: ProgressChanged2(int i) {
    LOGD("MyGLRender::ProgressChanged2 key = %d", i)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::ProgressChanged2() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->ProgressChanged2(i);
    }
}

void MyGLRender:: ProgressChanged3(int i) {
    LOGD("MyGLRender::ProgressChanged3 key = %d", i)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::ProgressChanged3() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->ProgressChanged3(i);
    }
}

void MyGLRender:: ProgressChanged4(int i) {
    LOGD("MyGLRender::ProgressChanged4 key = %d", i)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::ProgressChanged3() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->ProgressChanged4(i);
    }
}

void MyGLRender::MoveCallback(float x, float y) {
    LOGD("MyGLRender::MoveCallback x = %f, y = %f", x, y)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::MoveCallback() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->MoveCallback(x, y);
    }
}

