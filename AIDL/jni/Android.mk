# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libadr_hld
LOCAL_SRC_FILES := ./c_lib/libadr_hld.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := am_aidl
LOCAL_SRC_FILES := com_iptv_aidl_ALIAudioModeService.c
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDES += $(LOCAL_PATH)/hld_inc/inc/ $(LOCAL_PATH)/hld_inc/alidrivers/include
LOCAL_SHARED_LIBRARIES := libadr_hld libcutils

include $(BUILD_SHARED_LIBRARY)
