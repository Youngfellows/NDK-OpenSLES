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
    LOGI("filePath=%s\n",filePath);
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