LOCAL_PATH := $(call my-dir)

# First, build native_app_glue as a static library
include $(CLEAR_VARS)

LOCAL_MODULE := native_app_glue
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.c
LOCAL_EXPORT_C_INCLUDES := $(NDK_ROOT)/sources/android/native_app_glue

include $(BUILD_STATIC_LIBRARY)

# Now build the main shared library
include $(CLEAR_VARS)

LOCAL_MODULE := baseNativeLib
LOCAL_SRC_FILES := \
../app/src/main/cpp/Engine.cpp \
../app/src/main/cpp/custom/UI/Button.cpp \
../app/src/main/cpp/custom/UI/Container.cpp \
../app/src/main/cpp/custom/UI/UIManager.cpp

#Just add the different cpp files.
LOCAL_CFLAGS := -Wall -Werror -Wno-unused-function -Wno-unused-variable
LOCAL_CPPFLAGS := -std=gnu++17
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3
LOCAL_STATIC_LIBRARIES := native_app_glue

# Workaround to export ANativeActivity_onCreate
LOCAL_LDFLAGS := -u ANativeActivity_onCreate

LOCAL_C_INCLUDES := \
    $(NDK_ROOT)/sources/android/native_app_glue

include $(BUILD_SHARED_LIBRARY)
