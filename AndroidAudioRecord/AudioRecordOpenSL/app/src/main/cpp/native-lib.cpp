//
// Created by tck on 2019/7/17.
//

#include <jni.h>
#include <string>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "RecordBuffer.h"
#include "AndroidLog.h"

SLObjectItf slObjectEngine = NULL;
SLEngineItf engineItf = NULL;

SLObjectItf recordObj = NULL;
SLRecordItf recordItf = NULL;

SLAndroidSimpleBufferQueueItf recorderBufferQueue = NULL;

RecordBuffer *recordBuffer;

FILE *pcmFile = NULL;

bool finish = false;

void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {

    fwrite(recordBuffer->getNowBuffer(), 1, 4096, pcmFile);
    if(finish)
    {
        LOGE("录制完成");
        (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);
        //
        (*recordObj)->Destroy(recordObj);
        recordObj = NULL;
        recordItf = NULL;
        (*slObjectEngine)->Destroy(slObjectEngine);
        slObjectEngine = NULL;
        engineItf = NULL;
        delete(recordBuffer);
    } else{
        LOGE("正在录制");
        (*recorderBufferQueue)->Enqueue(recorderBufferQueue, recordBuffer->getRecordBuffer(), 4096);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_audiorecordopensl_AudioRecordUtil_startRecord(JNIEnv *env, jobject instance,
                                                           jstring path_) {
    if (finish) {
        return;
    }

    const char *path = env->GetStringUTFChars(path_, 0);
    finish = false;
    pcmFile = fopen(path, "w");
    recordBuffer = new RecordBuffer(4096);

    slCreateEngine(&slObjectEngine, 0, NULL, 0, NULL, NULL);

    (*slObjectEngine)->Realize(slObjectEngine, SL_BOOLEAN_FALSE);
    (*slObjectEngine)->GetInterface(slObjectEngine, SL_IID_ENGINE, &engineItf);

    SLDataLocator_IODevice loc_dev = {SL_DATALOCATOR_IODEVICE,
                                      SL_IODEVICE_AUDIOINPUT,
                                      SL_DEFAULTDEVICEID_AUDIOINPUT,
                                      NULL};
    SLDataSource audioSrc = {&loc_dev, NULL};

    SLDataLocator_AndroidSimpleBufferQueue loc_bq = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
            2
    };
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1,
            SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT, SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSink audioSnk = {&loc_bq, &format_pcm};

    const SLInterfaceID id[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    (*engineItf)->CreateAudioRecorder(engineItf, &recordObj, &audioSrc, &audioSnk, 1, id, req);
    (*recordObj)->Realize(recordObj, SL_BOOLEAN_FALSE);
    (*recordObj)->GetInterface(recordObj, SL_IID_RECORD, &recordItf);
    (*recordObj)->GetInterface(recordObj, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &recorderBufferQueue);

    (*recorderBufferQueue)->Enqueue(recorderBufferQueue, recordBuffer->getRecordBuffer(), 4096);
    (*recorderBufferQueue)->RegisterCallback(recorderBufferQueue, bqRecorderCallback, NULL);
    (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_RECORDING);
    env->ReleaseStringUTFChars(path_, path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tck_audiorecordopensl_AudioRecordUtil_stopRecord(JNIEnv *env, jobject instance) {
    //TODO:
    finish = true;
}