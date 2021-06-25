#NDK_TOOLCHAIN_VERSION := 4.9

#云游平台定义
# 0: Huawei(CG_HUAWEI)
# 1: Baidu(CG_BAIDU)
# 2: Intel(CG_INTEL)
GS_CG_PLATFORM = 0

ifeq ($(GS_CG_PLATFORM), 0)
# 0: HUAWEI
APP_ABI := arm64-v8a
APP_PLATFORM = android-24
else ifeq ($(GS_CG_PLATFORM), 1)
# 1: BAIDU
#APP_ABI := armeabi-v7a
APP_ABI := arm64-v8a
APP_PLATFORM = android-23
endif

# Get C++11 working
APP_CPPFLAGS += -std=c++11 -fexceptions -frtti 
# -D _GLIBCXX_USE_C99=1  #用于解决std::to_string在ndk下的报错
APP_STL := c++_static
#APP_STL := stlport_static
