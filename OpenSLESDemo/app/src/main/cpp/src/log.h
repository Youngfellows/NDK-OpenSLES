//
// Created by slience on 2019-09-27.
//

#ifndef OPENSLESDEMO_LOG_H
#define OPENSLESDEMO_LOG_H

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "AudioRecord_JNI"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


#endif //OPENSLESDEMO_LOG_H
