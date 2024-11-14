//
// Created by TS on 2024/2/27.
//

#include "MyGLRender.h"
#include "transform/Transform.h"
#include "transform/CoordinateSystems.h"
#include "transform/Camera.h"
#include "transform/CameraAutoMove.h"
#include "ColorsLight.h"
#include "ColorsAtView.h"
#include "Material.h"
#include "lighting_maps_diffuse.h"
#include "LightCastersDirectional.h"
#include "ModelLoading.h"
#include "multiple_lights.h"
#include "advanced_opengl/depth_testing/DepthTesting.h"
#include "advanced_opengl/stencil_testing/StencilTesting.h"
#include "advanced_opengl/blending_discard/BlendingDiscard.h"
#include "advanced_opengl/cull_face/CullFace.h"
#include "advanced_opengl/frame_buffer/FrameBuffers.h"
#include "advanced_opengl/frame_buffer/FrameBuffersExercise.h"
#include "advanced_opengl/cube_maps/CubeMaps.h"
#include "advanced_opengl/cube_maps/CubeMapsReflection.h"
#include "advanced_opengl/cube_maps/ReflectionMode.h"
#include "advanced_opengl/cube_maps/CubeMapsRefraction.h"
#include "advanced_opengl/cube_maps/CubeMapsReflectionExercise.h"
#include "advanced_opengl/AdvancedUbo.h"
#include "advanced_opengl/geometry_shader/GeometryShaderPoint.h"
#include "advanced_opengl/instancing/Instancing.h"
#include "advanced_opengl/instancing/Asteroids.h"
#include "advanced_opengl/anti_aliasing/AnitAliasing.h"
#include "5_advanced_lighting/1_blinn_phong/AdvancedLighting.h"
#include "5_advanced_lighting/2_gamma_correction/gamma_correction.h"
#include "5_advanced_lighting/3_shadow_mapping_depth/shadow_mapping_depth.h"
#include "5_advanced_lighting/4_shadow_mapping_base/shadow_mapping_base.h"
#include "5_advanced_lighting/5_normal_mapping/normal_mapping.h"
#include "5_advanced_lighting/6_parallax_mapping/parallax_mapping.h"
#include "5_advanced_lighting/7_steep_parallax_mapping/steep_parallax_mapping.h"
#include "5_advanced_lighting/8_parallax_occlusion_mapping/parallax_occlusion_mapping.h"
#include <NativeTriangle7.h>
#include <NativeTriangle6.h>
#include <NativeTriangle.h>
#include <NativeRectangle.h>
#include <NativeTriangle2.h>
#include <NativeTriangle3.h>
#include <NativeTriangle4.h>
#include <NativeTriangle5.h>
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

void MyGLRender::SetRenderType(int renderSampleType) {
    LOGD("MyGLRender::SetRenderType renderSampleType = %d", renderSampleType)
    LOGD("MyGLRenderContext::SetRenderType 0 m_pBeforeSample = %p", m_before_sample)
    m_before_sample = m_curr_sample;
    switch (renderSampleType) {
        case SAMPLE_TYPE_KEY_TRIANGLE:
            m_curr_sample = new NativeTriangle();
            break;
        case SAMPLE_TYPE_KEY_RECTANGLE:
            m_curr_sample = new NativeRectangle();
            break;
        case SAMPLE_TYPE_TRIANGLE2:
            m_curr_sample = new NativeTriangle2();
            break;
        case SAMPLE_TYPE_TRIANGLE3:
            m_curr_sample = new NativeTriangle3();
            break;
        case SAMPLE_TYPE_TRIANGLE4:
            m_curr_sample = new NativeTriangle4();
            break;
        case SAMPLE_TYPE_TRIANGLE5:
            m_curr_sample = new NativeTriangle5();
            break;
        case SAMPLE_TYPE_TEXTURE:
            m_curr_sample = new NativeTriangle6();
            break;
        case SAMPLE_TYPE_TEXTURE2:
            m_curr_sample = new NativeTriangle7();
            break;
        case SAMPLE_TYPE_MAT:
            m_curr_sample = new Transform();
            break;
        case SAMPLE_TYPE_COORDINATE:
            m_curr_sample = new CoordinateSystems();
            break;
        case SAMPLE_TYPE_CAMERA:
            m_curr_sample = new Camera();
            break;
        case SAMPLE_TYPE_CAMERA_AUTO_MOVE:
            m_curr_sample = new CameraAutoMove();
            break;
        case SAMPLE_TYPE_COLORS:
            m_curr_sample = new ColorsLight();
            break;
        case SAMPLE_TYPE_COLORS_VIEW:
            m_curr_sample = new ColorsAtView();
            break;
        case SAMPLE_TYPE_COLORS_MATERIAL:
            m_curr_sample = new Material();
            break;
        case SAMPLE_TYPE_LIGHTING_MAPS_DIFFUSE:
            m_curr_sample = new LightingMapsDiffuse();
            break;
        case SAMPLE_TYPE_LIGHTING_CASTERS_DIRECTIONAL:
            m_curr_sample = new LightCastersDirectional();
            break;
        case SAMPLE_TYPE_LIGHTING_MERGE:
            m_curr_sample = new MultipleLights();
            break;
        case SAMPLE_TYPE_MODEL_LOADING:
            m_curr_sample = new ModelLoading();
            break;
        case SAMPLE_TYPE_DEPTH_TESTING:
            m_curr_sample = new DepthTesting();
            break;
        case SAMPLE_TYPE_STENCIL_TESTING:
            m_curr_sample = new StencilTesting();
            break;
        case SAMPLE_TYPE_BLENDING_DISCARD:
            m_curr_sample = new BlendingDiscard();
            break;
        case SAMPLE_TYPE_CULL_FACE:
            m_curr_sample = new CullFace();
            break;
        case SAMPLE_TYPE_FRAME_BUFFERS:
            m_curr_sample = new FrameBuffers();
            break;
        case SAMPLE_TYPE_FRAME_BUFFERS_EXERCISE:
            m_curr_sample = new FrameBuffersExercise();
            break;
        case SAMPLE_TYPE_CUBE_MAPS:
            m_curr_sample = new CubeMaps();
            break;
        case SAMPLE_TYPE_CUBE_MAPS_REFLECTION1:
            m_curr_sample = new CubeMapsReflection();
            break;
        case SAMPLE_TYPE_CUBE_MAPS_REFLECTION2:
            m_curr_sample = new ReflectionMode();
            break;
        case SAMPLE_TYPE_CUBE_MAPS_REFRACTION:
            m_curr_sample = new CubeMapsRefraction();
            break;
        case SAMPLE_TYPE_CUBE_MAPS_EXERCISE:
            m_curr_sample = new CubeMapsReflectionExercise();
            break;
        case SAMPLE_TYPE_CUBE_UNIFORM_BUFFER:
            m_curr_sample = new AdvancedUbo();
            break;
        case SAMPLE_TYPE_GEOMETRY_SHADER:
            m_curr_sample = new GeometryShaderPoint();
            break;
        case SAMPLE_TYPE_INSTANCING:
            m_curr_sample = new Instancing();
            break;
        case SAMPLE_TYPE_INSTANCING_ASTEROIDS:
            m_curr_sample = new Asteroids();
            break;
        case SAMPLE_TYPE_ANIT_ALIASING:
            m_curr_sample = new AnitAliasing();
            break;
        case SAMPLE_TYPE_ADVANCED_LIGHTING:
            m_curr_sample = new AdvancedLighting();
            break;
        case SAMPLE_TYPE_ADVANCED_LIGHTING_GAMMA_CORRECTED:
            m_curr_sample = new gamma_correction();
            break;
        case SAMPLE_TYPE_SHADOW_MAPPING_DEPTH:
            m_curr_sample = new shadow_mapping_depth();
            break;
        case SAMPLE_TYPE_SHADOW_MAPPING_BASE:
            m_curr_sample = new shadow_mapping_base();
            break;
        case SAMPLE_TYPE_NORMAL_MAPPING:
            m_curr_sample = new normal_mapping();
            break;
        case SAMPLE_TYPE_PARALLAX_MAPPING:
            m_curr_sample = new parallax_mapping();
            break;
        case SAMPLE_TYPE_STEEP_PARALLAX_MAPPING:
            m_curr_sample = new steep_parallax_mapping();
            break;
        case SAMPLE_TYPE_PARALLAX_OCCLUSION_MAPPING:
            m_curr_sample = new parallax_occlusion_mapping();
            break;
        default:
            break;
    }
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::SetRenderType() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    }
    LOGD("MyGLRender::SetRenderType m_before_sample = %p, m_curr_sample=%p",
         m_before_sample, m_curr_sample)
}

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

void MyGLRender::MoveCallback(float x, float y) {
    LOGD("MyGLRender::MoveCallback x = %f，y = %f", x, y)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::MoveCallback() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->MoveCallback(x, y);
    }
}

