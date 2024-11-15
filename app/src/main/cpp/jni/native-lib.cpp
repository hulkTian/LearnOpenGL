#include <jni.h>
#include <MyGLRender.h>

extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeOnSurfaceCreated(JNIEnv *env, jobject thiz,
                                                                     jobject asset_manager) {
    MyGLRender::GetInstance()->OnSurfaceCreated(env, asset_manager);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeOnSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                     jint width, jint height) {
    MyGLRender::GetInstance()->OnSurfaceChanged(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeOnDrawFrame(JNIEnv *env, jobject thiz) {
    MyGLRender::GetInstance()->OnDrawFrame();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeSetRenderType(JNIEnv *env, jobject thiz,
                                                                  jint render_sample_type) {
    MyGLRender::GetInstance()->SetRenderType(render_sample_type);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeProcessInput(JNIEnv *env, jobject thiz,
                                                                 jint key) {
    MyGLRender::GetInstance()->ProcessInput(key);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeMoveCallback(JNIEnv *env, jobject thiz,
                                                                 jfloat x, jfloat y) {
    MyGLRender::GetInstance()->MoveCallback(x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeOnDestroy(JNIEnv *env, jobject thiz) {
    MyGLRender::DestroyInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_learnopengl_MyNativeRenderer_nativeProgressChanged(JNIEnv *env, jobject thiz,
                                                                    jint progress) {
    MyGLRender::GetInstance()->ProgressChanged(progress);
}