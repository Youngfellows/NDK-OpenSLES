//
// Created by zuo on 2022/5/30/030.
//

#include "OpenSLESPlayer.h"


FILE *pcmFile;
void *buffer;
uint8_t *out_buffer;
SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue = NULL;

OpenSLESPlayer::OpenSLESPlayer() {

}

OpenSLESPlayer::~OpenSLESPlayer() {

}

int64_t getPcmData(void **pcm, FILE *pcmFile, uint8_t *out_buffer) {
    while(!feof(pcmFile)) {
        size_t size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        *pcm = out_buffer;
        return size;
    }
    return 0;
}

// 6、启动回调函数
void pcmBufferCallBack(SLAndroidSimpleBufferQueueItf bf, void * context) {

    int32_t size = getPcmData(&buffer, pcmFile, out_buffer);
    LOGD("pcmBufferCallBack, size=%d", size);
    if (NULL != buffer && size > 0) {
        SLresult result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, buffer, size);
    }
}

int32_t OpenSLESPlayer::prepare() {
    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);

    // 1、创建引擎与接口
    SLresult ret = createEngine();
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("createEngine() failed.");
        return ret;
    }

    // 2、设置混音器
    //通过引擎接口创建混音器outputMixObject
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mreq[1] = {SL_BOOLEAN_FALSE};
    ret = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mreq);
//    ret = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0,0, 0);
    if(ret != SL_RESULT_SUCCESS) {
        LOGD("CreateOutputMix failed, ret=%d", ret);
        return ret;
    }

    // 初始化混音器outputMixObject
    ret = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if(ret != SL_RESULT_SUCCESS) {
        LOGD("Realize failed, result=%d", ret);
        return ret;
    }

    // 3、创建播放器
    /**
     * 配置输入声音信息
     */
    //创建buffer缓冲类型的队列作为数据定位器(获取播放数据) 2个缓冲区
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};

    //pcm数据格式: pcm、声道数、采样率、采样位、容器大小、通道掩码(双声道)、字节序(小端)
    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM,
                            2,
                            SL_SAMPLINGRATE_44_1,
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_PCMSAMPLEFORMAT_FIXED_16,
                            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
                            SL_BYTEORDER_LITTLEENDIAN};

    //数据源 将上述配置信息放到这个数据源中
    SLDataSource slDataSource = {&android_queue, &pcm};

    //设置混音器
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    //开启功能
    SLDataSink audioSnk = {&outputMix, NULL};
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    //创建播放器
    ret = (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource, &audioSnk, 1, ids, req);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("CreateAudioPlayer() failed.");
        return ret;
    }
    //初始化播放器
    ret = (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("pcmPlayerObject Realize() failed.");
        return ret;
    }

    // 4、设置播放回调
    //获得播放数据队列操作接口
    //注册回调缓冲区 //获取缓冲队列接口
    ret = (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("pcmPlayerObject GetInterface(SL_IID_BUFFERQUEUE) failed.");
        return ret;
    }

    LOGE("==RegisterCallback==%d,bqPlayerBufferQueue=%p",ret,bqPlayerBufferQueue);
    //设置回调(启动播放器后执行回调来获取数据并播放)
    ret = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, pcmBufferCallBack, this);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("SLAndroidSimpleBufferQueueItf RegisterCallback() failed.");
        return ret;
    }
    return ret;
}

int32_t OpenSLESPlayer::setDataSource(const std::string &pcmPath) {
    LOGE("setDataSource->%s",pcmPath.c_str());
    pcmFile = fopen(pcmPath.c_str(), "r");

    if (pcmFile == NULL) {
        LOGD("open path=%s failed.", pcmPath.c_str());
        return -1;
    }

    return 0;
}

SLresult OpenSLESPlayer::createEngine() {
    SLresult result;
    // 创建引擎engineObject
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        LOGD("slCreateEngine failed, result=%d", result);
        return result;
    }
    // 初始化引擎engineObject
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGD("engineObject Realize failed, result=%d", result);
        return result;
    }

    // 获取引擎接口engineInterface
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE,&engineEngine);
    if (SL_RESULT_SUCCESS != result) {
        LOGD("engineObject GetInterface failed, result=%d", result);
        return result;
    }
    return result;
}

// 5、设置播放状态
void OpenSLESPlayer::start() {
    //获取播放状态接口
    SLresult ret = (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);
    if (ret != SL_RESULT_SUCCESS) {
        LOGD("pcmPlayerObject GetInterface(SL_IID_PLAY) failed.");
        return;
    }

    //设置播放状态
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);
    // 主动调用回调函数开始工作
    pcmBufferCallBack(bqPlayerBufferQueue, this);
}

void OpenSLESPlayer::resume() {
    if (pcmPlayerPlay != nullptr) {
        (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);
    }
}

void OpenSLESPlayer::pause() {
    if (pcmPlayerPlay != nullptr) {
        (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PAUSED);
    }
}

void OpenSLESPlayer::stop() {
    LOGD("stop");
    if (pcmPlayerPlay != nullptr) {
        (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_STOPPED);
    }
}

// 7、停止播放
void OpenSLESPlayer::release() {
    LOGD("start release");
    //设置停止状态
    if (pcmPlayerPlay) {
        (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_STOPPED);
        pcmPlayerPlay = NULL;
    }
    //销毁播放器
    if (pcmPlayerObject) {
        (*pcmPlayerObject)->Destroy(pcmPlayerObject);
        pcmPlayerObject = NULL;
        bqPlayerBufferQueue = NULL;
        pcmFile = NULL;
        buffer = NULL;
        free(out_buffer);
        out_buffer = NULL;
    }
    //销毁混音器
    if (outputMixObject) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;

    }
    //销毁引擎
    if (engineObject) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}
