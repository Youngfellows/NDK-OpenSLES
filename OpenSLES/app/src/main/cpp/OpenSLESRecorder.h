//
// Created by zuo on 2022/5/31/031.
//

#ifndef OPENSLES_OPENSLESRECORDER_H
#define OPENSLES_OPENSLESRECORDER_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "AndroidLog.h"
#include <string>
#include <assert.h>
#include <sys/syscall.h>
#include <unistd.h>

#define NUM_BUFFER_QUEUE 1
#define SAMPLE_RATE 44100
#define NUM_RECORDER_EXPLICIT_INTERFACES 2
#define PERIOD_TIME 20  // 20ms
#define FRAME_SIZE SAMPLE_RATE * PERIOD_TIME / 1000
#define CHANNELS 2
#define BUFFER_SIZE   (FRAME_SIZE * CHANNELS)

class OpenSLESRecorder {
public:
    OpenSLESRecorder();
    ~OpenSLESRecorder();

    void startRecord(const std::string& path);

    void pauseRecord();

    void recordingRecord();

    void stopRecord();

    void release();

private:
    SLEngineItf engineEngine = NULL;
    SLObjectItf engineObject = NULL;
    SLObjectItf recorderObject = NULL; //录制对象，这个对象我们从里面获取了2个接口
    SLAndroidConfigurationItf configItf = NULL;
    SLAndroidSimpleBufferQueueItf recorderBuffQueueItf = NULL; //Buffer接口
    void createEngine();

public:
    uint8_t *buffer;
    FILE *pfile;
    size_t bufferSize;
    SLRecordItf recorderRecord = NULL; //录制接口
};


#endif //OPENSLES_OPENSLESRECORDER_H
