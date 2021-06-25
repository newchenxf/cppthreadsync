#ifndef LOG_UTILS_H
#define LOG_UTILS_H

#ifdef __ANDROID__
#include <android/log.h>
#endif

#include <unistd.h>
#include <stdarg.h>
#include <iostream>
#include "plog/Log.h"


#define ALOGI(...) PLOGI.printf(__VA_ARGS__)
#define ALOGW(...) PLOGW.printf(__VA_ARGS__)
#define ALOGE(...) PLOGE.printf(__VA_ARGS__)

void intializeLog(std::string mode="t", std::string logDirName="/home/chenxiaofeng", std::string logFileName="tdsync_test.txt");


#endif
