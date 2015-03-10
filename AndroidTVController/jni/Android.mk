LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := netcall
LOCAL_SRC_FILES := alinet.cpp mytcpip.cpp
LOCAL_LDLIBS    += -llog
include $(BUILD_SHARED_LIBRARY)
