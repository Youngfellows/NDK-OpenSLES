//
// Created by slience on 2019-09-27.
//

#include <jni.h>
#include "log.h"
#include "AudioRecorder.h"

AudioRecorder *audioRecorder = nullptr;

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_silence_audiorecord_AudioRecord__1startRecord(JNIEnv *env, jclass type,
                                                       jstring filePath_) {
    const char *filePath = env->GetStringUTFChars(filePath_, 0);
    LOGI("filePath=%s\n", filePath);
    if (audioRecorder) {
        audioRecorder->stop();
        delete audioRecorder;
    }
    audioRecorder = new AudioRecorder(filePath);

    env->ReleaseStringUTFChars(filePath_, filePath);

    return (jboolean) audioRecorder->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_silence_audiorecord_AudioRecord__1stopRecord(JNIEnv *env, jclass type) {

    if (!audioRecorder) {
        return;
    }
    audioRecorder->stop();
    delete audioRecorder;
    audioRecorder = nullptr;

}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    LOGI("%s,Line %d\n", __FUNCTION__, __LINE__);
    JNIEnv *env = NULL;
    jint result = -1;
    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    result = JNI_VERSION_1_4;
    return result;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOGI("%s,Line %d\n", __FUNCTION__, __LINE__);
}