//
// Created by Jesson on 2022/5/26.
//

#ifndef OPENSLESDEMO_ANDROIDLOG_H
#define OPENSLESDEMO_ANDROIDLOG_H

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"zxj",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"zxj",__VA_ARGS__)

#endif //OPENSLESDEMO_ANDROIDLOG_H
