//
// Created by HAPPY on 2020/3/9.
//

#ifndef NATIVEANDROID_NATIVEMAIN_H
#define NATIVEANDROID_NATIVEMAIN_H

#include <android/native_activity.h>
#include <android/input.h>

#include <jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <memory>
#include <android/native_window_jni.h>

//ANativeActivity * activity;
ANativeWindow *window;

void surfaceCreated(int color) {
    // strip RGBA precent
    GLfloat redF = ((color >> 16) & 0xFF) * 1.0f / 255;
    GLfloat greenF = ((color >> 8) & 0xFF) * 1.0f / 255;
    GLfloat blueF = (color & 0xFF) * 1.0f / 255;
    GLfloat alphaF = ((color >> 24) & 0xFF) * 1.0f / 255;
    glClearColor(redF, greenF, blueF, alphaF);
}

void surfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
}

void onDrawFrame() {
    // set color buffer
    glClear(GL_COLOR_BUFFER_BIT);
}

void draw(int width = 0, int height = 0) {
    // borrow from https://blog.csdn.net/byhook/article/details/84345512
    if (!window) {
        return;
    }
//    if (window) {
//        ANativeWindow_release(window);
//        window = 0;
//    }
// you can create new windows to show video
//    window = ANativeWindow_fromSurface(env, surface);
// set attribute of the Window

    ANativeWindow_setBuffersGeometry(window, width,
                                     height,
                                     WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        return;
    }
    if (!width || !height) {
        width = window_buffer.width;
        height = window_buffer.height;
    }
    LOGE("width:%d\theight:%d\n", width, height);
    // put rgb data to dst_data
    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            // The storage color order is BGR,BGR,BGR......
//            dst_data[column] = ((uint32_t) row_pointers[row][3 * column + 2]) << 16
//                           | ((uint32_t) row_pointers[row][3 * column + 1]) << 8
//                           | (uint32_t) row_pointers[row][3 * column];
            dst_data[column] = 6;
        }
        dst_data = dst_data + window_buffer.stride;
    }
    ANativeWindow_unlockAndPost(window);
}


void bindLifeCycle(ANativeActivity *activity);

// entry
void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize);


void *looper(void *args);

void *tickLooper(void *args);


void onStart(ANativeActivity *activity);

void onResume(ANativeActivity *activity);


void *onSaveInstanceState(ANativeActivity *activity, size_t *outSize);

void onPause(ANativeActivity *activity);

void onStop(ANativeActivity *activity);

void onDestroy(ANativeActivity *activity);

void onWindowFocusChanged(ANativeActivity *activity, int hasFocus);

void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *window);

void onNativeWindowDestroyed(ANativeActivity *activity, ANativeWindow *window);

void onInputQueueCreated(ANativeActivity *activity, AInputQueue *queue);

void onInputQueueDestroyed(ANativeActivity *activity, AInputQueue *queue);

void onConfigurationChanged(ANativeActivity *activity);

void onLowMemory(ANativeActivity *activity);

#endif //NATIVEANDROID_NATIVEMAIN_H
