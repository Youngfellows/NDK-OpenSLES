
#ifndef NATIVESAMPLEAUDIORECORD_AUDIOJNICALLBACK_H
#define NATIVESAMPLEAUDIORECORD_AUDIOJNICALLBACK_H

#include <jni.h>

typedef struct _AudioJniCallback {
    jmethodID method_data_callback;
    jobject java_object;
} AudioJniCallback;

#endif //NATIVESAMPLEAUDIORECORD_AUDIOJNICALLBACK_H
