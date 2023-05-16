//
// Created by slience on 2019-09-27.
//

#ifndef OPENSLESDEMO_AUDIOENGINE_H
#define OPENSLESDEMO_AUDIOENGINE_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <assert.h>

class AudioEngine {
public:
    SLObjectItf engineObj;
    SLEngineItf engine;

private:
    void createEngine() {
        SLresult result;

        result = slCreateEngine(&engineObj, 0, nullptr, 0, nullptr, nullptr);
        assert(result == SL_RESULT_SUCCESS);
        (void) result;

        result = (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
        assert(SL_RESULT_SUCCESS == result);
        (void) result;

        result = (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engine);
        assert(SL_RESULT_SUCCESS == result);
        (void) result;

    }

    virtual void release() {
        if (engineObj) {
            (*engineObj)->Destroy(engineObj);
            engineObj = nullptr;
            engine = nullptr;
        }
    }

public:
    AudioEngine() : engineObj(nullptr), engine(nullptr) {
        createEngine();
    }

    virtual ~AudioEngine() {
        release();
    }
};


#endif //OPENSLESDEMO_AUDIOENGINE_H
