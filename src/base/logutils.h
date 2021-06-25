#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#ifdef __ANDROID__
#include <android/log.h>
#endif

#include <unistd.h>
#include <stdarg.h>
#include <iostream>
//#include "plog/Log.h"

//#define LOG_TAG "iqiyi"

#define ALOGI(...) printf(__VA_ARGS__)
#define ALOGW(...) printf(__VA_ARGS__)
#define ALOGE(...) printf(__VA_ARGS__)

#endif
