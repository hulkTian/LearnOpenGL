//
// Created by TS on 2024/2/27.
//

#include "MyGLRender.h"
#include "Transform.h"
#include "CoordinateSystems.h"
#include "Camera.h"
#include "CameraAutoMove.h"
#include "light/ColorsLight.h"
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
        m_curr_sample->Create();
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
        m_curr_sample->Draw();
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

void MyGLRender::MoveCallback(double x, double y, double z) {
    LOGD("MyGLRender::MoveCallback x = %f，y = %f，z = %f", x, y, z)
    if (m_curr_sample == nullptr) {
        throw MyGLException(
                "MyGLRender::MoveCallback() 请注意：你应该忘记初始化你要展示的Sample类型 ，请补上初始化的代码，否则无法渲染");
    } else {
        m_curr_sample->MoveCallback(x, y, z);
    }
}

