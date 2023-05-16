//
// Created by slience on 2019-09-27.
//

#ifndef OPENSLESDEMO_AUDIORECORD_H
#define OPENSLESDEMO_AUDIORECORD_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <stdio.h>
#include "AudioEngine.h"
#include <queue>

class AudioRecorder {
private:
    FILE *mFile;

    AudioEngine *mAudioEngine;
    SLObjectItf mRecorderObj;
    SLRecordItf mRecorder;
    SLAndroidSimpleBufferQueueItf mBufferQueue;

    unsigned mBufSize;
    short *mBuffers[2];
    int mIndex;

    bool mIsRecording;
    bool mIsInitialized;

private:
    bool initRecorder();

    void release();

    static void recorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

public:
    AudioRecorder(const char *filePath);

    bool start();

    void stop();

    virtual ~AudioRecorder();
};


#endif //OPENSLESDEMO_AUDIORECORD_H
