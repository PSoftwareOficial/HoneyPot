LOCAL_PATH := $(call my-dir)

# -------------------------------
# Build native_app_glue as a static library
# -------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := native_app_glue
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.c
LOCAL_EXPORT_C_INCLUDES := $(NDK_ROOT)/sources/android/native_app_glue

include $(BUILD_STATIC_LIBRARY)

# -------------------------------
# Build main shared library
# -------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := baseNativeLib
LOCAL_SRC_FILES := \
    ../app/src/main/cpp/Engine/Engine.cpp \
    ../app/src/main/cpp/custom/UI/UIManager.cpp\
    ../app/src/main/cpp/custom/World.cpp\
    ../app/src/main/cpp/utilities/utilities.cpp\
    ../app/src/main/cpp/rendering/text/text.cpp

LOCAL_CFLAGS := -Wall -Wno-unused-function -Wno-unused-variable
LOCAL_CPPFLAGS := -std=gnu++17
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3
LOCAL_STATIC_LIBRARIES := native_app_glue

# Workaround to export ANativeActivity_onCreate
LOCAL_LDFLAGS := -u ANativeActivity_onCreate

LOCAL_C_INCLUDES := \
    $(NDK_ROOT)/sources/android/native_app_glue

include $(BUILD_SHARED_LIBRARY)