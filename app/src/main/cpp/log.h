//
// Created by HAPPY on 2020/3/9.
//

#ifndef NATIVEANDROID_LOG_H
#define NATIVEANDROID_LOG_H

#include <android/log.h>

#define  LOG_TAG    "native_android"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR ,LOG_TAG ,__VA_ARGS__)


#endif //NATIVEANDROID_LOG_H
