#include "com_speex_audiorecord_SpeexAudioCapture.h"
#include "AndroidLog.h"
#include <audiocapture.h>
#include "AudioJniCallback.h"
#include "jvm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHECK_CTX(ctx)  if (ctx == 0) return;

/**
 * 回调到java的结构体指针变量
 */
static AudioJniCallback *jniData;

/**
* 回调回来的PCM数据,可以通过jniData把数据回传会java层
* @param data
* @param size
* @param ctx
*/
void static readData(int8_t *data, uint32_t size, void *ctx) {
    //LOG_D("%s():: Line %d,size:%d\n", __FUNCTION__, __LINE__, size)
    CHECK_CTX(ctx)
    AudioCapture *capture = static_cast<AudioCapture *>(ctx);
    if (!jniData)
        return;
    bool thread = false; //记录当前是否存在jni环境
    JNIEnv *env = getEnv(&thread);
    //LOG_D("%s():: Line %d,thread:%d,size:%d\n", __FUNCTION__, __LINE__, thread, size)
    //将pcm数据封装成NIO buffer传输到java层
    jobject buffer = env->NewDirectByteBuffer(data, size * sizeof(int8_t));
    env->CallVoidMethod(jniData->java_object, jniData->method_data_callback, buffer);
    if (thread) //如果是新Attach线程则需要Detach
        detatchEnv();
}
/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _initRecord
 * Signature: (IIII)J
 */
JNIEXPORT jlong JNICALL
Java_com_speex_audiorecord_SpeexAudioCapture__1initRecord(JNIEnv *env, jobject instance,
                                                          jint captureType, jint sampleRate,
                                                          jint channelConfig,
                                                          jint audioFormat) {
    LOG_D("%s():: Line %d\n", __FUNCTION__, __LINE__)
    //开始录制
    //AudioCapture *capture = new AudioCapture(AUDIO_CAPTURE_TYPE_OPENSLES, AUDIO_SAMPLE_RATE_44_1,AUDIO_CHANNEL_MONO, AUDIO_FORMAT_PCM_16BIT);
    AudioCapture *capture = new AudioCapture(static_cast<uint16_t>(captureType),
                                             static_cast<uint32_t>(sampleRate),
                                             static_cast<uint16_t>(channelConfig),
                                             static_cast<uint16_t>(audioFormat));

    //将AudioCapture实例指针返回java存放
    return reinterpret_cast<intptr_t>(capture);
}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _stop
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1stop
        (JNIEnv *env, jobject instance, jlong ctx) {
    LOG_D("%s():: Line %d\n", __FUNCTION__, __LINE__)
    CHECK_CTX(ctx)
    AudioCapture *capture = reinterpret_cast<AudioCapture *>(ctx);
    if (capture->getState() == STATE_RECORDING) {
        capture->stopRecording();//结束录制
    }
}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _start
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1start
        (JNIEnv *env, jobject instance, jlong ctx) {
    LOG_D("%s():: Line %d\n", __FUNCTION__, __LINE__)
    CHECK_CTX(ctx)
    AudioCapture *capture = reinterpret_cast<AudioCapture *>(ctx);
    if (capture->getState() == STATE_IDEL) {
        capture->setAudioCaptureCallback(readData, capture);
        capture->startRecording();
    } else {
        capture->releaseRecording();
        delete capture;
        capture = nullptr;
    }
}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1release
        (JNIEnv *env, jobject instance, jlong ctx) {
    LOG_D("%s():: Line %d\n", __FUNCTION__, __LINE__)
    CHECK_CTX(ctx)
    AudioCapture *capture = reinterpret_cast<AudioCapture *>(ctx);
    if (capture->getState() != STATE_UNINIT) {
        capture->releaseRecording();
        delete capture;
        capture = nullptr;
    }
}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _getState
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1getState
        (JNIEnv *env, jobject instance, jlong ctx) {
    LOG_D("%s():: Line %d\n", __FUNCTION__, __LINE__)
    if (ctx == 0) {
        return STATE_UNINIT;
    }
    AudioCapture *capture = reinterpret_cast<AudioCapture *>(ctx);
    return capture->getState();
}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _setNativeCallback
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1setNativeCallback
        (JNIEnv *env, jobject instance, jlong ctx, jboolean need) {
    LOG_D("%s():: Line %d,need:%d\n", __FUNCTION__, __LINE__, need)
    if (need) {
        //设置回调函数
        jniData = new AudioJniCallback();
        jniData->java_object = getEnv()->NewGlobalRef(instance);
        jclass cls = getEnv()->GetObjectClass(instance);
        jniData->method_data_callback = getEnv()->GetMethodID(cls, "onNativeDataRead",
                                                              "(Ljava/nio/ByteBuffer;)V");
    }
}

//extern "C"
//jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
//    LOG_E("%s():: Line %d\n", __FUNCTION__, __LINE__)
//    JNIEnv *env = nullptr;
//    if (jvm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
//        return -1;
//    //初始化全局jvm，用于提供其他线程的jni环境
//    initGlobalJvm(jvm);
//    LOG_E("%s():: Line %d\n", __FUNCTION__, __LINE__)
//    return JNI_VERSION_1_6;
//}
//
//extern "C"
//JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
//    LOGI("%s,Line %d\n", __FUNCTION__, __LINE__);
//}

#ifdef __cplusplus
}
#endif
