//
// Created by zuo on 2022/5/30/030.
//

#ifndef OPENSLES_OPENSLESPLAYER_H
#define OPENSLES_OPENSLESPLAYER_H

#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "AndroidLog.h"

class OpenSLESPlayer {
public:
    OpenSLESPlayer();
    ~OpenSLESPlayer();
    int32_t prepare();
    int32_t setDataSource(const std::string& pcmPath);
    void start();
    void resume();
    void pause();
    void stop();
    void release();
private:
    SLresult createEngine();
private:
    SLObjectItf engineObject = NULL;
    SLEngineItf engineEngine= NULL;

    SLObjectItf outputMixObject = NULL;
    SLObjectItf pcmPlayerObject = NULL;
    SLPlayItf pcmPlayerPlay = NULL;
    SLVolumeItf playerVolume = NULL;
};


#endif //OPENSLES_OPENSLESPLAYER_H
