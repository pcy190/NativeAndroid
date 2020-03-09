#include <android/log.h>
#include <pthread.h>
#include <memory>
#include <thread>
#include "log.h"
#include "NativeMain.h"

//#define LOG_TAG "MAIN_CPP"

static bool isLoop = false;
static pthread_t loopID;
static pthread_t tickLoopID;


void onStart(ANativeActivity *activity) {
    LOGE("Application is start");
}

void onResume(ANativeActivity *activity) {
    LOGE("On resume");
    isLoop = true;
}

void *onSaveInstanceState(ANativeActivity *activity, size_t *outSize) {
    *outSize = 0;
    return nullptr;
}

void onPause(ANativeActivity *activity) {
    LOGI("onPause");
}

void onStop(ANativeActivity *activity) {
    LOGI("onStop");
    isLoop = false;
}

void onDestroy(ANativeActivity *activity) {
    LOGI("onDestroy");
//    activity->vm->DetachCurrentThread();
//    ANativeActivity_finish(activity);
}

void onWindowFocusChanged(ANativeActivity *activity, int hasFocus) {
    LOGE("on Windows Focus Changed");
}

void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *windows) {
    window = windows;
    LOGI("On native window created");
}

void onNativeWindowDestroyed(ANativeActivity *activity, ANativeWindow *windows) {
    window = nullptr;
    LOGI("On native window destroyed");
}

void onInputQueueCreated(ANativeActivity *activity, AInputQueue *queue) {
    isLoop = true;
    activity->instance = (void *) queue;
    pthread_create(&loopID, nullptr, looper, activity);
    pthread_create(&tickLoopID, nullptr, tickLooper, activity);
}

void onInputQueueDestroyed(ANativeActivity *activity, AInputQueue *queue) {
    isLoop = false;
    LOGI("onInputQueueDestroyed");
}

void onConfigurationChanged(ANativeActivity *activity) {

}

void onLowMemory(ANativeActivity *activity) {

}

void bindLifeCycle(ANativeActivity *activity) {
    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;
}

void *looper(void *args) {
    auto *activity = (ANativeActivity *) args;
    auto *queue = (AInputQueue *) activity->instance;
    AInputEvent *event = nullptr;
    while (isLoop) {
        if (!AInputQueue_hasEvents(queue)) {
            continue;
        }
        AInputQueue_getEvent(queue, &event);
        float mx = AMotionEvent_getX(event, 0);
        float my = AMotionEvent_getY(event, 0);
        switch (AInputEvent_getType(event)) {
            case AINPUT_EVENT_TYPE_MOTION: {
                switch (AMotionEvent_getAction(event)) {
                    case AMOTION_EVENT_ACTION_DOWN: {
                        LOGE("Touch Screen Down");
                        break;
                    }
                    case AMOTION_EVENT_ACTION_UP: {
                        LOGE("Touch Screen UP");
                        break;
                    }
                    case AMOTION_EVENT_BUTTON_BACK: {
                        LOGE("Press Back Button");
                        activity->vm->DetachCurrentThread();
                        ANativeActivity_finish(activity);
                        break;
                    }
                    case AMOTION_EVENT_BUTTON_SECONDARY: {
                        LOGE("AMOTION_EVENT_BUTTON_SECONDARY");
                        break;
                    }
                    case AMOTION_EVENT_BUTTON_STYLUS_SECONDARY: {
                        LOGE("AMOTION_EVENT_BUTTON_SECONDARY");
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case AINPUT_EVENT_TYPE_KEY: {
                switch (AKeyEvent_getAction(event)) {
                    case AKEY_EVENT_ACTION_DOWN: {
                        LOGE("key down");
                        switch (AKeyEvent_getKeyCode(event)) {
                            case AKEYCODE_BACK: {
                                LOGE("BACK down");
                                ANativeActivity_finish(activity);
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    }
                    case AKEY_EVENT_ACTION_UP: {
                        LOGE("key up");
                        break;
                    }
                    default:
                        break;
                }
            }
            default:
                break;
        }
        AInputQueue_finishEvent(queue, event, 1);
    }
    return args;
}

void *tickLooper(void *args) {
    while (isLoop) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
//        this->application->update(*gl_context);
        draw();
    }
    return args;
}

void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize) {
    LOGE("init LifeCycle");
    bindLifeCycle(activity);
}