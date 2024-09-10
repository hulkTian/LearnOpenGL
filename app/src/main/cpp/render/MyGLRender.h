//
// Created by TS on 2024/2/27.
//

#ifndef LEARNOPENGL_MYGLRENDER_H
#define LEARNOPENGL_MYGLRENDER_H

#include <GLBaseSample.h>

/**
 * native渲染器：
 *
 */
class MyGLRender {
    MyGLRender();

    ~MyGLRender();

public:
    void SetRenderType(int renderSampleType);

    void ProcessInput(int i);

    void MoveCallback(double x, double y);

    void OnSurfaceCreated(JNIEnv *env, jobject assetManager);

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    static MyGLRender* GetInstance();

    static void DestroyInstance();

private:
    static MyGLRender *m_instance;
    GLBaseSample *m_before_sample;
    GLBaseSample *m_curr_sample;
};

#endif //LEARNOPENGL_MYGLRENDER_H
