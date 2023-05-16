#include <jni.h>
#include <string>
#include "OpenSLESPlayer.h"
#include "OpenSLESRecorder.h"

OpenSLESPlayer *player = NULL;
OpenSLESRecorder *audioRecorder = NULL;

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESPlayer_init(JNIEnv *env, jobject thiz) {
    LOGD("init");
    if (player == NULL) {
        player = new OpenSLESPlayer();
    }
    player->prepare();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESPlayer_setPcmData(JNIEnv *env, jobject thiz, jstring path) {
//    LOGD("setPcmData->path:%s",path);
    const char *filePath = env->GetStringUTFChars(path,0);
    if (player == NULL) {
        player = new OpenSLESPlayer();
    }
    player->setDataSource(filePath);
//    player->prepare();
    player->start();
    env->ReleaseStringUTFChars(path,filePath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESPlayer_pause(JNIEnv *env, jobject thiz) {
    if (player != NULL) {
        player->pause();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESPlayer_resume(JNIEnv *env, jobject thiz) {
    if (player != NULL) {
        player->resume();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESPlayer_stop(JNIEnv *env, jobject thiz) {
    LOGD("OpenSlESPlayer_stop");
    if (player != NULL) {
        player->stop();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESPlayer_release(JNIEnv *env, jobject thiz) {
    LOGD("OpenSlESPlayer_release");
    if (player != NULL) {
        player->stop();
        player->release();
        delete (player);
        player = NULL;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESAudioRecorder_startRecord(JNIEnv *env, jobject thiz, jstring path) {
    LOGD("startRecord->path:%s",path);
    const char *filePath = env->GetStringUTFChars(path,0);
    LOGE("filePath->%s",filePath);
    if(filePath == nullptr){
        LOGD("filePath == NULL");
        filePath = "/sdcard/audio.pcm";
    }
    if(audioRecorder == nullptr){
        audioRecorder = new OpenSLESRecorder();
    }
    audioRecorder->startRecord(filePath);
    env->ReleaseStringUTFChars(path,filePath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESAudioRecorder_recordingRecord(JNIEnv *env, jobject thiz) {
    LOGD("OpenSlESAudioRecorder_recorderRecord");
    if(audioRecorder != nullptr){
        audioRecorder->recordingRecord();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESAudioRecorder_pauseRecord(JNIEnv *env, jobject thiz) {
    LOGD("OpenSlESAudioRecorder_pauseRecord");
    if(audioRecorder != nullptr){
        audioRecorder->pauseRecord();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESAudioRecorder_stopRecord(JNIEnv *env, jobject thiz) {
    LOGD("OpenSlESAudioRecorder_stopRecord");
    if(audioRecorder != nullptr){
        audioRecorder->stopRecord();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_zxj_opensles_OpenSlESAudioRecorder_release(JNIEnv *env, jobject thiz) {
    LOGD("===OpenSlESAudioRecorder->release===");
    if(audioRecorder != nullptr){
        audioRecorder->release();
        delete audioRecorder;
        audioRecorder = nullptr;
    }
}