#include "com_speex_audiorecord_SpeexAudioCapture.h"
#include "AndroidLog.h"
#include <audiocapture.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
* PCM数据回调
* @param data
* @param size
* @param ctx
*/
void static readData(int8_t *data, uint32_t size, void *ctx) {
    //TODO
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
    //开始录制
    AudioCapture *capture = new AudioCapture(AUDIO_CAPTURE_TYPE_OPENSLES, AUDIO_SAMPLE_RATE_44_1,
                                             AUDIO_CHANNEL_MONO, AUDIO_FORMAT_PCM_16BIT);
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
 * Method:    _stop
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1stop
        (JNIEnv *, jobject, jlong) {

}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _start
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1start
        (JNIEnv *, jobject, jlong) {

}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1release
        (JNIEnv *, jobject, jlong) {

}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _getState
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1getState
        (JNIEnv *, jobject, jlong) {

}

/*
 * Class:     com_speex_audiorecord_SpeexAudioCapture
 * Method:    _setNativeCallback
 * Signature: (JZ)V
 */
JNIEXPORT void JNICALL Java_com_speex_audiorecord_SpeexAudioCapture__1setNativeCallback
        (JNIEnv *, jobject, jlong, jboolean) {

}


#ifdef __cplusplus
}
#endif
