
LOCAL_PATH := $(call my-dir)/..


# main binary
include $(CLEAR_VARS)


LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../src \
    $(LOCAL_PATH)/../include \

LOCAL_CPP_FILES := $(wildcard $(LOCAL_PATH)/../src/base/*.cpp)
LOCAL_CPP_FILES += $(wildcard $(LOCAL_PATH)/../src/util/*.cpp)
LOCAL_CPP_FILES += $(wildcard $(LOCAL_PATH)/../src/debug/*.cpp)
LOCAL_CPP_FILES += $(wildcard $(LOCAL_PATH)/../src/*.cpp)
LOCAL_SRC_FILES := $(LOCAL_CPP_FILES)


LOCAL_MODULE := mockService

LOCAL_LDFLAGS += -llog

include $(BUILD_EXECUTABLE)
